// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_BROWSER_TAB_WIDGET_H
#define QCEF_BROWSER_TAB_WIDGET_H

#include <QTabWidget>
#include <QUrl>
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

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 private:
  BrowserTabWidgetPrivate* p_ = nullptr;

 private slots:
  void onCurrentChanged(int index);

  // Hide tab bar in fullscreen mode.
  void onFullscreenRequested(bool fullscreen);

  // Close web view at |index|.
  void onTabCloseRequested(int index);
};


#endif //CEF_BROWSER_TAB_WIDGET_H
