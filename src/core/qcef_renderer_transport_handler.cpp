/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#include "core/qcef_renderer_transport_handler.h"

#include <string>

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

  browser_->SendProcessMessage(PID_BROWSER, msg);

  return true;
}
