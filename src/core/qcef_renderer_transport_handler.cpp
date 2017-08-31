// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_renderer_transport_handler.h"

#include <QDebug>

#include "core/qcef_web_channel_consts.h"
#include "include/wrapper/cef_helpers.h"

QCefRendererTransportHandler::QCefRendererTransportHandler(
    CefRefPtr<CefBrowser> browser)
    : browser_(browser) {
}

bool QCefRendererTransportHandler::Execute(const CefString& name,
                                           CefRefPtr<CefV8Value> object,
                                           const CefV8ValueList& arguments,
                                           CefRefPtr<CefV8Value>& retval,
                                           CefString& exception) {
  CEF_REQUIRE_RENDERER_THREAD();
  DCHECK_EQ(name, "send");

  // Send message from renderer process to browser process.
  CefRefPtr<CefProcessMessage> msg =
      CefProcessMessage::Create(kQCefRenderQtMessage);

  CefRefPtr<CefListValue> args = msg->GetArgumentList();
  args->SetString(0, arguments[0]->GetStringValue());

#ifdef N_DEBUG
  qDebug() << "QCefRendererTransportHandler:Execute() "
           << "render send normal qt message:"
           << std::string(arguments[0]->GetStringValue()).c_str();
#endif

  browser_->SendProcessMessage(PID_BROWSER, msg);

  return true;
}
