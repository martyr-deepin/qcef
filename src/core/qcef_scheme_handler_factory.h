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

#ifndef QCEF_CORE_QCEF_SCHEME_HANDLER_FACTORY_H
#define QCEF_CORE_QCEF_SCHEME_HANDLER_FACTORY_H

#include "core/qcef_scheme_handler.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_stream_resource_handler.h"

// Handle custom scheme requests.
class QCefSchemeHandlerFactory : public CefSchemeHandlerFactory {
 public:
  explicit QCefSchemeHandlerFactory(QCefSchemeHandler handler)
      : custom_scheme_handler_(handler) { }

  CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       const CefString& scheme_name,
                                       CefRefPtr<CefRequest> request) override;

 private:
  IMPLEMENT_REFCOUNTING(QCefSchemeHandlerFactory);

  QCefSchemeHandler custom_scheme_handler_ = nullptr;
};

#endif  // QCEF_CORE_QCEF_SCHEME_HANDLER_FACTORY_H