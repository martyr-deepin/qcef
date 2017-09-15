// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_context.h"

#include <glib.h>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "core/qcef_app.h"
#include "core/qcef_x11_util.h"
#include "core/qcef_cookie_store.h"
#include "include/cef_path_util.h"

namespace {

gboolean ProcessQtEvent(gpointer user_data) {
  Q_UNUSED(user_data);
  if (qApp != nullptr) {
    qApp->processEvents();
  }
  return TRUE;
}

}  // namespace

int QCefInit(int argc, char** argv, const QCefGlobalSettings& settings) {
  SetXErrorHandler();

  int cef_argc = argc;
  char** cef_argv = nullptr;
  cef_argv = static_cast<char**>(calloc(static_cast<size_t>(cef_argc),
                                        sizeof(argv[0])));
  memcpy(cef_argv, argv, argc * sizeof(argv[0]));

  CefMainArgs main_args(cef_argc, cef_argv);
  CefRefPtr<QCefApp> client_app(new QCefApp());

  // Add flash plugin parameters.
  QCefApp::AppendedArguments arguments;
  if (settings.pepperFlash()) {
    arguments.push_back({"ppapi-flash-path",
                         settings.getPepperFlashPath()});
    arguments.push_back({"ppapi-flash-version",
                         settings.getPepperFlashVersion()});
  }
  switch (settings.proxyType()) {
    case QCefGlobalSettings::ProxyType::NoProxy: {
      arguments.push_back({"--no-proxy-server", ""});
      break;
    }
    case QCefGlobalSettings::ProxyType::PacUrl: {
      arguments.push_back({"--proxy-pac-url", settings.proxyInfo()});
      break;
    }
    case QCefGlobalSettings::ProxyType::ProxyServer: {
      arguments.push_back({"--proxy-server", settings.proxyInfo()});
      break;
    }
    case QCefGlobalSettings::ProxyType::AutoDetect: {
      arguments.push_back({"--proxy-auto-detect", ""});
      break;
    }
    default: {
      break;
    }
  }

  client_app->appendCommandLineSwitches(arguments);

  client_app->addCustomSchemes(settings.customSchemes());

  client_app->setCustomSchemeHandler(settings.getCustomSchemeHandler());

  client_app->setSyncMethods(settings.getSyncMethods());

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

void QCefBindApp(QCoreApplication* app) {
  auto timer = new QTimer();
  QObject::connect(app, &QCoreApplication::aboutToQuit, [timer]() {
    QCefFlushCookies();
    CefShutdown();
    timer->stop();
    timer->deleteLater();
  });
  QObject::connect(timer, &QTimer::timeout, []() {
    CefDoMessageLoopWork();
  });
  timer->setInterval(1);
  timer->start();
}

void QCefRunLoop() {
  // Register event dispatcher.
  g_timeout_add(50, ProcessQtEvent, nullptr);

  CefRunMessageLoop();

  // Shutdown loop internally.
  CefShutdown();
}

void QCefQuitLoop() {
  // Flash global cookie.
  QCefFlushCookies();

  // Close cef context within 300ms.
  g_timeout_add(300, [](gpointer user_data) -> gboolean {
    Q_UNUSED(user_data);
    CefQuitMessageLoop();
    return FALSE;
  }, nullptr);
}
