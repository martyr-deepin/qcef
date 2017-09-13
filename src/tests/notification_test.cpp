// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>

#include "tests/notification_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  NotificationWindow window;
  window.resize(640, 480);
  window.show();

  return app.exec();
}