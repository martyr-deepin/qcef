// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_renderer_handler.h"

#include <QDebug>

#include "base/file_util.h"
#include "core/qcef_web_channel_consts.h"
#include "core/qcef_renderer_transport_handler.h"
#include "core/qcef_sync_method_handler.h"
#include "include/wrapper/cef_helpers.h"

namespace {

void RegisterRendererTransport(CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) {
  context->Enter();

  // Register qt.webChannelTransport.send() method.
  CefRefPtr<CefV8Value> transport = CefV8Value::CreateObject(nullptr, nullptr);
  CefRefPtr<CefV8Handler> send_handler =
      new QCefRendererTransportHandler(frame->GetBrowser());
  transport->SetValue("send",
                      CefV8Value::CreateFunction("send", send_handler),
                      V8_PROPERTY_ATTRIBUTE_NONE);
  CefRefPtr<CefV8Value> qt = CefV8Value::CreateObject(nullptr, nullptr);
  qt->SetValue("webChannelTransport", transport, V8_PROPERTY_ATTRIBUTE_NONE);
  context->GetGlobal()->SetValue("qt", qt, V8_PROPERTY_ATTRIBUTE_NONE);

  context->Exit();

  // Register QWebChannel class to global context.
  const QString content = ReadFile(":/qtwebchannel/qwebchannel.js");
  Q_ASSERT(!content.isEmpty());
  const CefString code = content.toStdWString();
  frame->ExecuteJavaScript(code, "qrc://qtwebchannel/qwebchannel.js", 0);
}

}  // namespace

QCefRendererHandler::QCefRendererHandler(const QCefSyncMethodMap& map)
    : sync_methods_(map) {
}

void QCefRendererHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                           CefRefPtr<CefFrame> frame,
                                           CefRefPtr<CefV8Context> context) {
  CEF_REQUIRE_RENDERER_THREAD();

  // Register sync methods.
  auto sync_handler = new QCefSyncMethodHandler(sync_methods_);

  context->Enter();
  // Retrieve the context's window object.
  CefRefPtr<CefV8Value> window = context->GetGlobal();
  for (const QString& function_name : sync_methods_.keys()) {
    const std::string name = function_name.toStdString();
    CefRefPtr<CefV8Value> function = CefV8Value::CreateFunction(name,
                                                                sync_handler);
    window->SetValue(name, function, V8_PROPERTY_ATTRIBUTE_NONE);
  }
  context->Exit();

  // Register qt web channel transport.
  if (frame->GetIdentifier() != browser->GetMainFrame()->GetIdentifier()) {
    return;
  }
  RegisterRendererTransport(frame, context);

  CefRefPtr<CefProcessMessage> msg =
      CefProcessMessage::Create(kQCefRenderContextCreated);
//  CefRefPtr<CefListValue> args = msg->GetArgumentList();
  browser->SendProcessMessage(PID_BROWSER, msg);
}

void QCefRendererHandler::OnContextReleased(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefV8Context> context) {
  (void) context;
  CEF_REQUIRE_RENDERER_THREAD();

  if (frame->GetIdentifier() != browser->GetMainFrame()->GetIdentifier()) {
    return;
  }

  CefRefPtr<CefProcessMessage> msg =
      CefProcessMessage::Create(kQCefRenderContextReleased);
//  CefRefPtr<CefListValue> args = msg->GetArgumentList();
  browser->SendProcessMessage(PID_BROWSER, msg);
}

bool QCefRendererHandler::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) {
  (void) source_process;
  CEF_REQUIRE_RENDERER_THREAD();

  const std::string name = std::string(message->GetName());
  if (name == kQCefRenderQtMessage) {
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    if (frame == nullptr) {
      qWarning() << __FUNCTION__ << "main frame is null!";
      return false;
    }

    CefRefPtr<CefListValue> args = message->GetArgumentList();
    if (args->GetSize() != 1) {
      qWarning() << __FUNCTION__ << "args size mismatch, expect 1!";
      return false;
    }
    CefRefPtr<CefV8Context> context = frame->GetV8Context();
    context->Enter();

    auto window = context->GetGlobal();
    auto transport = window->GetValue("qt")->GetValue("webChannelTransport");
    auto handler = transport->GetValue("onmessage");

    const std::string data = args->GetString(0);
    qDebug() << "Renderer received message from browser: " << data.c_str();
    CefV8ValueList vlist;
    CefRefPtr<CefV8Value> resp = CefV8Value::CreateObject(nullptr, nullptr);
    resp->SetValue("data",
                   CefV8Value::CreateString(args->GetString(0)),
                   V8_PROPERTY_ATTRIBUTE_NONE);
    vlist.push_back(resp);
    handler->ExecuteFunction(transport, vlist);

    context->Exit();

    return true;
  }

  return false;
}

void QCefRendererHandler::OnWebKitInitialized() {
}