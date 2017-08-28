// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>
#include <QTimer>
#include <qcef_context.h>
#include <qcef_global_settings.h>

#include "browser_window.h"
#include "sync_methods.h"

int main(int argc, char* argv[]) {
  QCefGlobalSettings settings;
  settings.setNoSandbox(true);
//  settings.setPepperFlash(true);
  settings.setRemoteDebug(true);
  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Info);

  // Register echoMessage in web page.
  settings.registerSyncMethod("echoMessage", EchoMessage);
  QCefInit(argc, argv, settings);

  QApplication app(argc, argv);
  BrowserWindow browser_window;
  browser_window.show();
  browser_window.resize(860, 640);

  QCefRunLoop();
  return 0;
}