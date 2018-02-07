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

#ifndef QCEF_CORE_QCEF_SYNC_METHOD_HANDLER_H
#define QCEF_CORE_QCEF_SYNC_METHOD_HANDLER_H

#include "core/qcef_sync_method.h"
#include "include/cef_v8.h"

class QCefSyncMethodHandler : public CefV8Handler {
 public:
  QCefSyncMethodHandler(const QCefSyncMethodMap& map);

  bool Execute(const CefString& name, CefRefPtr<CefV8Value> object,
               const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval,
               CefString& exception) override;

 private:
  IMPLEMENT_REFCOUNTING(QCefSyncMethodHandler);
  QCefSyncMethodMap sync_methods_;
};


#endif  // QCEF_CORE_QCEF_SYNC_METHOD_HANDLER_H
