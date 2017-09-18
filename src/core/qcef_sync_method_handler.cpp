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

#include "core/qcef_sync_method_handler.h"

QCefSyncMethodHandler::QCefSyncMethodHandler(const QCefSyncMethodMap& map)
  : sync_methods_(map) {
}

bool QCefSyncMethodHandler::Execute(const CefString& name,
                                    CefRefPtr<CefV8Value> object,
                                    const CefV8ValueList& arguments,
                                    CefRefPtr<CefV8Value>& retval,
                                    CefString& exception) {
  const QString func_name = QString::fromStdString(name.ToString());
  if (sync_methods_.contains(func_name)) {
    if (arguments.size() != 1) {
      exception = "Arguments mismatch, expect 1 argument!";
      return true;
    }
    const auto arg0 = arguments.at(0);
    if (!arg0->IsString()) {
      exception = "Argument 0 shall be a string!";
      return true;
    }
    const QString input = QString::fromStdString(arg0->GetStringValue());
    const QString result = sync_methods_[func_name](input);
    retval = CefV8Value::CreateString(result.toStdString());
    return true;
  }

  return false;
}
