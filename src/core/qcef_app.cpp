// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "core/qcef_app.h"

#include "core/qcef_print_handler.h"
#include "core/qcef_message_pump_handler.h"
#include "core/qcef_renderer_handler.h"
#include "core/qcef_scheme_handler_factory.h"
#include "include/wrapper/cef_helpers.h"

QCefApp::QCefApp() : message_handler_(new QCefMessagePumpHandler()) {
}

QCefApp::~QCefApp() {
  if (message_handler_ != nullptr) {
    message_handler_->deleteLater();
    message_handler_ = nullptr;
  }
}

void QCefApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  // Register custom scheme handler factory.
  CefRefPtr<QCefSchemeHandlerFactory> factory =
      new QCefSchemeHandlerFactory(custom_scheme_handler_);
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
  for (const QCefCommandLineSwitch& arg : appended_args_) {
    if (arg.value.isEmpty()) {
      command_line->AppendSwitch(arg.key.toStdString());
    } else {
      command_line->AppendSwitchWithValue(arg.key.toStdString(),
                                          arg.value.toStdString());
    }
  }
}

void QCefApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) {
  // Register file:/ and qrc:/ schemes.
  registrar->AddCustomScheme("file", true, true, false, true, true, false);
  registrar->AddCustomScheme("qrc", false, true, false, true, true, false);

  // Register custom scheme names.
  if (!custom_scheme_list_.empty()) {
    for (const QUrl& entry : custom_scheme_list_) {
      registrar->AddCustomScheme(entry.scheme().toStdString(),
                                 true, true, false, true, true, false);
    }
  }
}

void QCefApp::appendCommandLineSwitches(const QCefCommandLineSwitchList& args) {
  appended_args_ = args;
}

CefRefPtr<CefRenderProcessHandler> QCefApp::GetRenderProcessHandler() {
  return new QCefRendererHandler(sync_methods_, register_scripts_);
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

void QCefApp::setRegisterScripts(const QCefUserScriptList& scripts) {
  register_scripts_ = scripts;
}
