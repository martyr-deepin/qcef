/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QDebug>
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
  settings.setRemoteDebug(true);
  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Info);

  QDir cache_dir(QDir::home().filePath(".cache/qcef/browser-demo"));
  cache_dir.mkpath(".");
  settings.setUserDataPath(cache_dir.filePath("data"));
  settings.setCachePath(cache_dir.filePath("cache"));
  settings.setLogFile(cache_dir.filePath("web-console.log"));

  // Register echoMessage in web page.
  settings.registerSyncMethod("echoMessage", EchoMessage);

  // Register user script.
  settings.registerUserScript(":/resources/user_script.js",
                              QUrl("qrc://resources/user_script.js"));

  // Disable GPU process.
  settings.addCommandLineSwitch("--disable-gpu", "");

  // Enable aggressive storage commit to minimize data loss.
  // See public/common/content_switches.cc.
  settings.addCommandLineSwitch("--enable-aggressive-domstorage-flushing", "");

  const int exit_code = QCefInit(argc, argv, settings);
  if (exit_code >= 0) {
    return exit_code;
  }

  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/images/firefox.png"));

  qCritical() << "Qt:" << QT_VERSION << QT_VERSION_MAJOR << QT_VERSION_MINOR;

  BrowserWindow browser_window;
  browser_window.resize(860, 640);
  browser_window.show();

  QCefBindApp(&app);
  return app.exec();
}
