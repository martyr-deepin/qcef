// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
