// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_context.h"

#include <glib-2.0/glib.h>
#include <QCoreApplication>
#include <QDebug>

#include "core/qcef_app.h"
#include "core/qcef_util.h"
#include "include/cef_path_util.h"

namespace {

gboolean ProcessQtEvent(gpointer user_data) {
  Q_UNUSED(user_data);
  qApp->processEvents();
  return TRUE;
}

}  // namespace

int QCefInit(int argc, char* argv[], const QCefGlobalSettings& settings) {
  SetXErrorHandler();

  CefMainArgs main_args(argc, argv);
  CefRefPtr<QCefApp> client_app(new QCefApp());

  // Add flash plugin parameters.
  if (settings.pepperFlash()) {
    QCefApp::AppendedArguments arguments;
    arguments.push_back({"ppapi-flash-path",
                         settings.getPepperFlashPath().toStdString()});
    arguments.push_back({"ppapi-flash-version",
                         settings.getPepperFlashVersion().toStdString()});
    client_app->appendCommandLineSwitches(arguments);
  }

  QCefApp::CustomSchemeList custom_scheme_list;
  for (const QUrl& url : settings.customSchemes()) {
    QCefApp::CustomSchemeEntry entry = {
        url.scheme().toStdString(),
        url.host().toStdString()
    };
    custom_scheme_list.push_back(entry);
  }
  client_app->addCustomSchemes(custom_scheme_list);

  client_app->setCustomSchemeHandler(settings.getCustomSchemeHandler());

#ifdef QCEF_OVERRIDE_PATH
  if (!CefOverridePath(PK_DIR_EXE, QCEF_OVERRIDE_PATH)) {
    qCritical() << "Failed to override PK_DIR_EXE";
  }
  if (!CefOverridePath(PK_DIR_MODULE, QCEF_OVERRIDE_PATH)) {
    qCritical() << "Failed to override PK_DIR_MODULE";
  }
#endif

  // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
  // that share the same executable. This function checks the command-line and,
  // if this is a sub-process, executes the appropriate logic.
  int exit_code = CefExecuteProcess(main_args, client_app.get(), nullptr);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  // Merge CEF global settings.
  CefSettings cef_settings;
  cef_settings.single_process = settings.singleProcess() ? 1 : 0;
  cef_settings.no_sandbox = settings.noSandbox() ? 1 : 0;
  cef_settings.command_line_args_disabled =
      settings.disableCommandLineArgs() ? 1 : 0;
  CefString(&cef_settings.cache_path) = settings.cachePath().toStdString();
  CefString(&cef_settings.user_data_path) =
      settings.userDataPath().toStdString();
  cef_settings.persist_session_cookies =
      settings.persistSessionCookies() ? 1 : 0;
  cef_settings.persist_user_preferences =
      settings.persistUserPreferences() ? 1 : 0;
  CefString(&cef_settings.user_agent) = settings.userAgent().toStdString();
  CefString(&cef_settings.log_file) = settings.logFile().toStdString();
  switch (settings.logSeverity()) {
    case QCefGlobalSettings::LogSeverity::Default: {
      cef_settings.log_severity = LOGSEVERITY_DEFAULT;
      break;
    }
    case QCefGlobalSettings::LogSeverity::Verbose: {
      cef_settings.log_severity = LOGSEVERITY_VERBOSE;
      break;
    }
    case QCefGlobalSettings::LogSeverity::Info: {
      cef_settings.log_severity = LOGSEVERITY_INFO;
      break;
    }
    case QCefGlobalSettings::LogSeverity::Warning: {
      cef_settings.log_severity = LOGSEVERITY_WARNING;
      break;
    }
    case QCefGlobalSettings::LogSeverity::Error: {
      cef_settings.log_severity = LOGSEVERITY_ERROR;
      break;
    }
    case QCefGlobalSettings::LogSeverity::Disable: {
      cef_settings.log_severity = LOGSEVERITY_DISABLE;
      break;
    }
  }
  if (settings.remoteDebug()) {
    cef_settings.remote_debugging_port = settings.remoteDebugPort();
  }
  cef_settings.ignore_certificate_errors =
      settings.ignoresCertificateErrors() ? 1 : 0;
  CefString(&cef_settings.accept_language_list) =
      settings.acceptLanguageList().toStdString();

  // Initialize CEF for the browser process.
  if (!CefInitialize(main_args, cef_settings, client_app.get(), nullptr)) {
    qCritical() << "CefInitialize() failed!";
    return 1;
  }

  return 0;
}

void QCefRunLoop() {
  g_timeout_add(50, ProcessQtEvent, nullptr);
  CefRunMessageLoop();

  // Shutdown loop internally.
  CefShutdown();
}

void QCefQuitLoop() {
  CefQuitMessageLoop();
}
