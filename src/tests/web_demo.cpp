/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#include "core/qcef_context.h"
#include "widgets/qcef_web_view.h"

std::ostream& operator<<(std::ostream& ostream, wchar_t const* content) {
  ostream << content;
  return ostream;
}

int main(int argc, char** argv) {
  QCefGlobalSettings settings;
  // Do not use sandbox.
  settings.setNoSandbox(true);

  // Open http://localhost:9222 in chromium browser to see dev tools.
  settings.setRemoteDebug(true);
  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Info);

  const int exit_code = QCefInit(argc, argv, settings);
  if (exit_code >= 0) {
    return exit_code;
  }

  QApplication app(argc, argv);
  QCefBindApp(&app);

  QCefWebView view;
  view.show();
  view.setUrl(QUrl("https://www.bing.com"));

  return app.exec();
}