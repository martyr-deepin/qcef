// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_NOTIFICATION_CONSTRUCTOR_H
#define QCEF_CORE_QCEF_NOTIFICATION_CONSTRUCTOR_H

#include "include/cef_v8.h"

// Implementation of Notification() prototype constructor.
class QCefNotificationConstructor : public CefV8Handler {
 public:
  QCefNotificationConstructor(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame);
  bool Execute(const CefString& name,
               CefRefPtr<CefV8Value> object,
               const CefV8ValueList& arguments,
               CefRefPtr<CefV8Value>& retval,
               CefString& exception) override;

 private:
  IMPLEMENT_REFCOUNTING(QCefNotificationConstructor);
  DISALLOW_COPY_AND_ASSIGN(QCefNotificationConstructor);

  CefRefPtr<CefBrowser> browser_ = nullptr;
  CefRefPtr<CefFrame> frame_ = nullptr;
};

// Implementation of Notification.requestPermission() method.
class QCefNotificationRequestPermission : public CefV8Handler {
 public:
  QCefNotificationRequestPermission(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame);
  bool Execute(const CefString& name,
               CefRefPtr<CefV8Value> object,
               const CefV8ValueList& arguments,
               CefRefPtr<CefV8Value>& retval,
               CefString& exception) override;

 private:
 IMPLEMENT_REFCOUNTING(QCefNotificationRequestPermission);
  DISALLOW_COPY_AND_ASSIGN(QCefNotificationRequestPermission);

  CefRefPtr<CefBrowser> browser_ = nullptr;
  CefRefPtr<CefFrame> frame_ = nullptr;
};

#endif  // QCEF_CORE_QCEF_NOTIFICATION_CONSTRUCTOR_H
