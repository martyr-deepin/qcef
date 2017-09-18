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
