// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_RENDERER_TRANSPORT_HANDLER_H
#define QCEF_CORE_QCEF_RENDERER_TRANSPORT_HANDLER_H

#include "include/cef_v8.h"

// Send message from renderer process to browser process.
class QCefRendererTransportHandler : public CefV8Handler {
 public:
  explicit QCefRendererTransportHandler(CefRefPtr<CefBrowser> browser);

  bool Execute(const CefString& name,
               CefRefPtr<CefV8Value> object,
               const CefV8ValueList& arguments,
               CefRefPtr<CefV8Value>& retval,
               CefString& exception) override;

  IMPLEMENT_REFCOUNTING(QCefRendererTransportHandler);

 private:
  CefRefPtr<CefBrowser> browser_ = nullptr;
};

#endif  // QCEF_CORE_QCEF_RENDERER_TRANSPORT_HANDLER_H
