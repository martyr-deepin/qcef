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

#ifndef QCEF_BROWSER_TAB_WIDGET_H
#define QCEF_BROWSER_TAB_WIDGET_H

#include <QTabWidget>
#include <QUrl>
#include <qcef_browser_event_delegate.h>
#include <qcef_ssl_status.h>

struct BrowserTabWidgetPrivate;

class BrowserTabWidget : public QTabWidget {
  Q_OBJECT

 public:
  explicit BrowserTabWidget(QWidget* parent = nullptr);
  ~BrowserTabWidget() override;

  // Get loading state of current web view.
  bool isLoading() const;

  QCefSSLStatus getSSlStatus() const;

 signals:
  void fullscreenRequested(bool fullscreen);
  void urlChanged(const QUrl& url);

  void loadingStateChanged(bool is_loading,
                           bool can_go_back,
                           bool can_go_forward);

 public slots:
  void createNewBrowser(bool in_background, const QUrl& url = QUrl());

  // Control navigation of current web page.
  void back();
  void forward();
  void reload();
  void stop();

  // Load |url| in current web view.
  void load(const QUrl& url);

  void zoomIn();
  void zoomOut();
  void zoomReset();

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 private:
  void initConnections();

  BrowserTabWidgetPrivate* p_ = nullptr;

 private slots:
  void onCurrentChanged(int index);

  // Hide tab bar in fullscreen mode.
  void onFullscreenRequested(bool fullscreen);

  void onNotificationReceived(const QString& summary, const QString& body);

  // Close web view at |index|.
  void onTabCloseRequested(int index);

  void onRefreshRequested();
  void onPopupRequested(const QUrl& url,
                        QCefWindowOpenDisposition disposition);
  void onToggleFullscreen();

  void setClipboardUrl(const QUrl& url);
};


#endif //CEF_BROWSER_TAB_WIDGET_H
