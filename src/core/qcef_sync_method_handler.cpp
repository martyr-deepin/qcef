// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
