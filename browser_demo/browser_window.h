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

#ifndef QCEF_BROWSER_DEMO_BROWSER_WINDOW_H
#define QCEF_BROWSER_DEMO_BROWSER_WINDOW_H

#include <QFrame>

class QCefWebView;
struct BrowserWindowPrivate;

class BrowserWindow : public QFrame {
  Q_OBJECT

 public:
  explicit BrowserWindow(QWidget* parent = nullptr);
  ~BrowserWindow() override;

 private:
  void initConnections();
  void initUI();

  BrowserWindowPrivate* p_ = nullptr;

 private slots:
  void onAddressEditActivated();
  void onReloadButtonClicked();

  void onFullscreenRequested(bool fullscreen);
  void onLoadingStateChanged(bool is_loading,
                             bool can_go_back,
                             bool can_go_forward);
  void onUrlChanged(const QUrl& url);

  void showHomePage();
};

#endif  // QCEF_BROWSER_DEMO_BROWSER_WINDOW_H
