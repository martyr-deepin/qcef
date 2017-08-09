// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_context.h"

#include <glib-2.0/glib.h>
#include <QApplication>
#include <QDebug>
#include <QTimer>

#include "core/qcef_app.h"
#include "core/qcef_util.h"

namespace {

bool g_message_loop_inited = false;

gboolean ProcessQtEvent(gpointer user_data) {
  Q_UNUSED(user_data);
  qApp->processEvents();
  return TRUE;
}

}  // namespace

bool AttachToQtMessageLoop() {
  if (g_message_loop_inited) {
    return false;
  }

  QTimer* timer = new QTimer();
  timer->setInterval(10);
  QObject::connect(timer, &QTimer::timeout, []() {
    CefDoMessageLoopWork();
  });
  timer->start();

  QObject::connect(qApp, &QApplication::aboutToQuit,
                   timer, &QTimer::deleteLater);

  g_message_loop_inited = true;
  return true;
}

int QCefInit(int argc, char* argv[], const QCefGlobalSettings& settings) {
  SetXErrorHandler();

  CefMainArgs main_args(argc, argv);
  CefRefPtr<QCefApp> client_app(new QCefApp());

  // Add flash plugin parameters.
  if (settings.pepperFlash()) {
    QCefApp::AppendedArguments arguments;
    arguments.push_back({"ppapi-flash-path", settings.getPepperFlashPath()});
    arguments.push_back({"ppapi-flash-version",
                         settings.getPepperFlashVersion()});
    client_app->appendCommandLineSwitches(arguments);
  }

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
  CefString(&cef_settings.cache_path) = settings.cachePath();
  CefString(&cef_settings.user_data_path) = settings.userDataPath();
  CefString(&cef_settings.user_agent) = settings.userAgent();
  CefString(&cef_settings.log_file) = settings.logFile();
  switch (settings.logSeverity()) {
    case QCefGlobalSettings::LogSeverity::Default: {
      cef_settings.log_severity = LOGSEVERITY_DEFAULT;
    }
    case QCefGlobalSettings::LogSeverity::Verbose: {
      cef_settings.log_severity = LOGSEVERITY_VERBOSE;
    }
    case QCefGlobalSettings::LogSeverity::Info: {
      cef_settings.log_severity = LOGSEVERITY_INFO;
    }
    case QCefGlobalSettings::LogSeverity::Warning: {
      cef_settings.log_severity = LOGSEVERITY_WARNING;
    }
    case QCefGlobalSettings::LogSeverity::Error: {
      cef_settings.log_severity = LOGSEVERITY_ERROR;
    }
    case QCefGlobalSettings::LogSeverity::Disable: {
      cef_settings.log_severity = LOGSEVERITY_DISABLE;
    }
  }
  if (settings.remoteDebug()) {
    cef_settings.remote_debugging_port = settings.remoteDebugPort();
  }
  cef_settings.ignore_certificate_errors =
      settings.ignoresCertificateErrors() ? 1 : 0;
  CefString(&cef_settings.accept_language_list) = settings.acceptLanguageList();

  // Initialize CEF for the browser process.
  if (!CefInitialize(main_args, cef_settings, client_app.get(), nullptr)) {
    qCritical() << "CefInitialize() failed!";
    return 1;
  }

  return 0;
}

void QCefRunLoop() {
  g_timeout_add(100, ProcessQtEvent, nullptr);
  QObject::connect(qApp, &QApplication::aboutToQuit, []() {
    qDebug() << "QApp about to quit";
    QCefQuitLoop();
  });
  CefRunMessageLoop();

  // Shutdown loop internally.
  CefShutdown();
}

void QCefQuitLoop() {
  CefQuitMessageLoop();
}
