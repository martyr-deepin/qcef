// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "core/qcef_app.h"

#include "core/qcef_renderer_handler.h"
#include "include/wrapper/cef_helpers.h"

QCefApp::QCefApp() : appended_args_(), scheme_list_() {

}

void QCefApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  LOG(WARNING) << "OnContextInitialized()";
}

void QCefApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {
  (void) process_type;
  for (const std::pair<std::string, std::string>& argument : appended_args_) {
    command_line->AppendSwitchWithValue(argument.first, argument.second);
  }
}

void QCefApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) {
  // Register file:// scheme.
  registrar->AddCustomScheme("file", true, true, false, true, true, false);
  for (const std::string& scheme : scheme_list_) {
    registrar->AddCustomScheme(scheme, true, true, false, true, true, false);
  }
}

void QCefApp::appendCommandLineSwitches(const AppendedArguments& args) {
  appended_args_ = args;
}

CefRefPtr<CefRenderProcessHandler> QCefApp::GetRenderProcessHandler() {
  return new QCefRendererHandler();
}

void QCefApp::addCustomSchemes(const QCefApp::SchemeList& scheme_list) {
  scheme_list_ = scheme_list;
}
