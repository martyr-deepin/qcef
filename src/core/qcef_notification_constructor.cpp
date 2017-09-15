// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
