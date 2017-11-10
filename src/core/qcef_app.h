// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef QCEF_CORE_QCEF_APP_H
#define QCEF_CORE_QCEF_APP_H

#include <QVector>

#include "core/qcef_global_settings.h"
#include "core/qcef_scheme_handler.h"
#include "core/qcef_sync_method.h"
#include "include/cef_app.h"

class QCefMessagePumpHandler;

// Implement application-level callbacks for the browser process.
class QCefApp : public CefApp,
                public CefBrowserProcessHandler {
 public:
  QCefApp();
  ~QCefApp() override;

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

  CefRefPtr<CefPrintHandler> GetPrintHandler() override;

#ifdef QCEF_EXTERNAL_MESSAGE_PUMP
  void OnScheduleMessagePumpWork(int64 delay_ms) override;
#endif  // QCEF_EXTERNAL_MESSAGE_PUMP

  // Open API used to customize cef app.
  void appendCommandLineSwitches(const QCefCommandLineSwitchList& args);

  void addCustomSchemes(const QList<QUrl>& list);

  void setCustomSchemeHandler(QCefSchemeHandler handler);

  void setSyncMethods(const QCefSyncMethodMap& map);

  void setRegisterScripts(const QCefUserScriptList& scripts);

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(QCefApp);
  DISALLOW_COPY_AND_ASSIGN(QCefApp);

  QCefCommandLineSwitchList appended_args_;
  QList<QUrl> custom_scheme_list_;
  QCefSchemeHandler custom_scheme_handler_ = nullptr;
  QCefSyncMethodMap sync_methods_;
  QCefUserScriptList register_scripts_;
  QCefMessagePumpHandler* message_handler_ = nullptr;
};

#endif  // QCEF_CORE_QCEF_APP_H
