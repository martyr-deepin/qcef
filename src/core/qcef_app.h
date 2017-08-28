// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef QCEF_CORE_QCEF_APP_H
#define QCEF_CORE_QCEF_APP_H

#include "include/cef_app.h"

#include "core/qcef_scheme_handler.h"
#include "core/qcef_sync_method.h"

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

  void addCustomSchemes(const QList<QUrl>& list);

  void setCustomSchemeHandler(QCefSchemeHandler handler);

  void setSyncMethods(const QCefSyncMethodMap& map);

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(QCefApp);

  AppendedArguments appended_args_;
  QList<QUrl> custom_scheme_list_;
  QCefSchemeHandler custom_scheme_handler_ = nullptr;
  QCefSyncMethodMap sync_methods_;
};

#endif  // QCEF_CORE_QCEF_APP_H