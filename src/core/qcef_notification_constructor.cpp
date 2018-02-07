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

#include "core/qcef_notification_constructor.h"

#include "core/qcef_web_channel_consts.h"

QCefNotificationConstructor::QCefNotificationConstructor(
    CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
    : browser_(browser),
      frame_(frame) {
}

bool QCefNotificationConstructor::Execute(const CefString& name,
                                          CefRefPtr<CefV8Value> object,
                                          const CefV8ValueList& arguments,
                                          CefRefPtr<CefV8Value>& retval,
                                          CefString& exception) {
  (void)name;
  (void)object;
  (void)retval;

  if (arguments.empty()) {
    exception = "Argument mismatch!";
    return true;
  }

  // Send notification message to UI thread.
  CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(
      kQCefWebNotificationBody);
  CefRefPtr<CefListValue> args = message->GetArgumentList();
  args->SetString(0, frame_->GetURL());
  args->SetString(1, arguments.at(0)->GetStringValue());
  if (arguments.size() > 1) {
    // Pass through notification option.
    CefRefPtr<CefV8Value> v8_value = arguments.at(1);
    std::vector<CefString> v8_keys;
    CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
    if (v8_value->GetKeys(v8_keys)) {
      for (const CefString& key : v8_keys) {
        CefRefPtr<CefV8Value> v = v8_value->GetValue(key);
        dict->SetString(key, v->GetStringValue());
      }
      args->SetDictionary(2, dict);
    }
  }
  browser_->SendProcessMessage(PID_BROWSER, message);

  return true;
}

QCefNotificationRequestPermission::QCefNotificationRequestPermission(
    CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
    : browser_(browser),
      frame_(frame) {
}

bool QCefNotificationRequestPermission::Execute(const CefString& name,
                                                CefRefPtr<CefV8Value> object,
                                                const CefV8ValueList& arguments,
                                                CefRefPtr<CefV8Value>& retval,
                                                CefString& exception) {
  (void)name;
  (void)object;
  (void)arguments;
  (void)retval;
  (void)exception;
  // TODO(LiuLang): Implement notification permission management.
  return true;
}
