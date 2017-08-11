// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef QCEF_CORE_QCEF_APP_H
#define QCEF_CORE_QCEF_APP_H

#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class QCefApp : public CefApp,
                public CefBrowserProcessHandler {
 public:
  QCefApp();

  // CefApp methods:
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
    return this;
  }
  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

  // CefBrowserProcessHandler methods:
  void OnContextInitialized() override;

  void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) override;

  void OnRegisterCustomSchemes(
      CefRawPtr<CefSchemeRegistrar> registrar) override;

  // Open API used to customize cef app.
  typedef std::vector<std::pair<std::string, std::string>> AppendedArguments;
  // Append |args| to command line.
  void appendCommandLineSwitches(const AppendedArguments& args);

  typedef std::vector<std::string> SchemeList;
  void addCustomSchemes(const SchemeList& scheme_list);

  struct CrossOriginEntry {
    std::string src_scheme;
    std::string src_host;
    std::string target_scheme;
    std::string target_host;
  };
  typedef std::vector<CrossOriginEntry> CrossOriginList;
  void addCrossOriginWhiteList(const CrossOriginList& list);

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(QCefApp);

  AppendedArguments appended_args_;
  SchemeList scheme_list_;
  CrossOriginList cross_origin_white_list_;
};

#endif  // QCEF_CORE_QCEF_APP_H
