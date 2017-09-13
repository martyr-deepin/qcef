// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>
#include <QDir>
#include <QIcon>
#include <qcef_context.h>
#include <qcef_util.h>

#include "browser_window.h"
#include "sync_methods.h"

#ifndef N_DEBUG
// operation overloading is missing in debug mode.
std::ostream& operator<<(std::ostream& os, wchar_t const* c) {
  os << c;
  return os;
}
#endif

namespace {

const char kPlatformThemeName[] = "QT_QPA_PLATFORMTHEME";
const char kGtk2Theme[] = "gtk2";

}  // namespace

int main(int argc, char* argv[]) {
  // If platform theme name is empty, fallback to gtk2.
  // gtk2 theme is included in libqt5libqgtk2 package.
  if (qgetenv(kPlatformThemeName) != kGtk2Theme) {
    qputenv(kPlatformThemeName, kGtk2Theme);
  }

  QCefGlobalSettings settings;
  // Do not use sandbox.
  settings.setNoSandbox(true);

  // Flash plugin only works on x86 platform.
  if (IsX86Architecture()) {
    // Pepper flash plugin crashes on chromium 60
//    settings.setPepperFlash(true);
  }

  // Use socks5 proxy.
//  settings.setProxyServer("socks5://127.0.0.1:1080");

  // Open http://localhost:9222 in chromium browser to see dev tools.
//  settings.setRemoteDebug(true);
//  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Info);

  QDir cache_dir(QDir::home().filePath(".cache/qcef/browser-demo"));
  cache_dir.mkpath(".");
  settings.setUserDataPath(cache_dir.filePath("data"));
  settings.setCachePath(cache_dir.filePath("cache"));
  settings.setLogFile(cache_dir.filePath("web-console.log"));

  // Register echoMessage in web page.
  settings.registerSyncMethod("echoMessage", EchoMessage);
  QCefInit(argc, argv, settings);

  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/images/firefox.png"));

  BrowserWindow browser_window;
  browser_window.resize(860, 640);
  browser_window.show();

  QCefBindApp(&app);
  return app.exec();
//  QCefRunLoop();
//
//  return 0;
}
