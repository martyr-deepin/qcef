// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <sys/utsname.h>
#include <QApplication>
#include <QTimer>
#include <qcef_context.h>
#include <qcef_global_settings.h>

#include "browser_window.h"
#include "sync_methods.h"

namespace {

const char kPlatformThemeName[] = "QT_QPA_PLATFORMTHEME";
const char kGtk2Theme[] = "gtk2";

bool IsX86Architecture() {
  struct utsname info;
  if (uname(&info) == 0) {
    const QString machine(info.machine);
    if (machine == "i386" || machine == "i468" || machine == "i586" ||
        machine == "i686" || machine == "x86" ||
        machine == "amd64" || machine == "x86_64") {
      return true;
    }
  }
  return false;
}

}  // namespace

int main(int argc, char* argv[]) {
  // If platform theme name is empty, fallback to gtk2.
  // gtk2 theme is included in libqt5libqgtk2 package.
  if (qgetenv(kPlatformThemeName) != kGtk2Theme) {
    qputenv(kPlatformThemeName, kGtk2Theme);
  }

  QCefGlobalSettings settings;
  settings.setNoSandbox(true);
  // Flash plugin only works on x86 platform.
  if (IsX86Architecture()) {
    settings.setPepperFlash(true);
  }
  settings.setRemoteDebug(true);
  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Info);

  // Register echoMessage in web page.
  settings.registerSyncMethod("echoMessage", EchoMessage);
  QCefInit(argc, argv, settings);

  QApplication app(argc, argv);
  BrowserWindow browser_window;
  browser_window.show();
  browser_window.resize(860, 640);
  browser_window.load(QUrl("qrc://resources/index.html"));

  QCefRunLoop();
  return 0;
}
