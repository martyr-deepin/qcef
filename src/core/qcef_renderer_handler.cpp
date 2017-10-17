/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "core/qcef_renderer_handler.h"

#include <string>

#include "base/file_util.h"
#include "core/qcef_notification_constructor.h"
#include "core/qcef_renderer_transport_handler.h"
#include "core/qcef_sync_method_handler.h"
#include "core/qcef_web_channel_consts.h"
#include "include/wrapper/cef_helpers.h"

namespace {

const char kWebChannelScriptFile[] = ":/qtwebchannel/qwebchannel.js";
const char kWebChannelScriptUrl[] = "qrc://qtwebchannel/qwebchannel.js";

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
  const QString content = ReadFile(kWebChannelScriptFile);
  Q_ASSERT(!content.isEmpty());
  const std::string code = content.toStdString();
  frame->ExecuteJavaScript(code, kWebChannelScriptUrl, 0);
}

}  // namespace

QCefRendererHandler::QCefRendererHandler(const QCefSyncMethodMap& map,
                                         const QCefUserScriptList& scripts)
    : sync_methods_(map),
      register_scripts_(scripts) {
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

  // Register the Notification javascript object.
  CefRefPtr<CefV8Value> notificationObj = CefV8Value::CreateFunction(
      "Notification",
      new QCefNotificationConstructor(browser, frame));

  // TODO(LiuLang): implement notification permission.
  notificationObj->SetValue("permission",
                            CefV8Value::CreateString("granted"),
                            V8_PROPERTY_ATTRIBUTE_NONE);
  auto request_permission = CefV8Value::CreateFunction(
      "requestPermission",
      new QCefNotificationRequestPermission(browser, frame));
  notificationObj->SetValue("requestPermission",
                            request_permission,
                            V8_PROPERTY_ATTRIBUTE_NONE);
  context->GetGlobal()->SetValue("Notification",
                                 notificationObj,
                                 V8_PROPERTY_ATTRIBUTE_NONE);
  context->Exit();

  // Register qt web channel transport.
  if (frame->GetIdentifier() != browser->GetMainFrame()->GetIdentifier()) {
    return;
  }
  RegisterRendererTransport(frame, context);

  // Register user scripts.
  for (const QCefUserScript& script : register_scripts_) {
    const QString content = ReadFile(script.path);
    Q_ASSERT(!content.isEmpty());
    if (!content.isEmpty()) {
      const std::string code = content.toStdString();
      const std::string url = script.url.toString().toStdString();
      frame->ExecuteJavaScript(code, url, 0);
    }
  }

  CefRefPtr<CefProcessMessage> msg =
      CefProcessMessage::Create(kQCefRenderContextCreated);
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
      LOG(ERROR) << __FUNCTION__ << "main frame is null!";
      return false;
    }

    CefRefPtr<CefListValue> args = message->GetArgumentList();
    if (args->GetSize() != 1) {
      LOG(ERROR) << __FUNCTION__ << "args size mismatch, expect 1!";
      return false;
    }
    CefRefPtr<CefV8Context> context = frame->GetV8Context();
    context->Enter();

    auto window = context->GetGlobal();
    auto transport = window->GetValue("qt")->GetValue("webChannelTransport");
    auto handler = transport->GetValue("onmessage");

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
