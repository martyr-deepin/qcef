// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_SCHEME_HANDLER_FACTORY_H
#define QCEF_CORE_QCEF_SCHEME_HANDLER_FACTORY_H

#include "core/qcef_scheme_handler.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_stream_resource_handler.h"

// Handle custom scheme requests.
class QCefSchemeHandlerFactory : public CefSchemeHandlerFactory {
 public:
  CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       const CefString& scheme_name,
                                       CefRefPtr<CefRequest> request) override;

  void setCustomSchemeHandler(QCefSchemeHandler handler);

 private:
  IMPLEMENT_REFCOUNTING(QCefSchemeHandlerFactory);

  QCefSchemeHandler custom_scheme_handler_ = nullptr;
};

#endif  // QCEF_CORE_QCEF_SCHEME_HANDLER_FACTORY_H