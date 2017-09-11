// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "core/qcef_app.h"

#include <set>

#include "core/qcef_print_handler.h"
#include "core/qcef_renderer_handler.h"
#include "core/qcef_scheme_handler_factory.h"
#include "include/wrapper/cef_helpers.h"

QCefApp::QCefApp() {

}

void QCefApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  // Register custom scheme handler factory.
  const auto factory = new QCefSchemeHandlerFactory();
  factory->setCustomSchemeHandler(custom_scheme_handler_);

  CefRegisterSchemeHandlerFactory("qrc", "", factory);

  for (const QUrl& entry : custom_scheme_list_) {
    CefRegisterSchemeHandlerFactory(entry.scheme().toStdString(),
                                    entry.host().toStdString(),
                                    factory);
  }
}

void QCefApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {
  (void)process_type;
  for (const QPair<QString, QString>& argument : appended_args_) {
    command_line->AppendSwitchWithValue(argument.first.toStdString(),
                                        argument.second.toStdString());
  }
}

void QCefApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) {
  // Register file:/ and qrc:/ schemes.
  registrar->AddCustomScheme("file", true, true, false, true, true, false);
  registrar->AddCustomScheme("qrc", true, true, false, true, true, false);

  // Register custom scheme names.
  if (!custom_scheme_list_.empty()) {
    for (const QUrl& entry : custom_scheme_list_) {
      registrar->AddCustomScheme(entry.scheme().toStdString(),
                                 true, true, false, true, true, false);
    }
  }
}

void QCefApp::appendCommandLineSwitches(const AppendedArguments& args) {
  appended_args_ = args;
}

CefRefPtr<CefRenderProcessHandler> QCefApp::GetRenderProcessHandler() {
  return new QCefRendererHandler(sync_methods_);
}

void QCefApp::addCustomSchemes(const QList<QUrl>& list) {
  custom_scheme_list_ = list;
}

void QCefApp::setCustomSchemeHandler(QCefSchemeHandler handler) {
  custom_scheme_handler_ = handler;
}

void QCefApp::setSyncMethods(const QCefSyncMethodMap& map) {
  sync_methods_ = map;
}

CefRefPtr<CefPrintHandler> QCefApp::GetPrintHandler() {
  return new QCefPrintHandler();
}
