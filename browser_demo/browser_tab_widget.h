// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_BROWSER_TAB_WIDGET_H
#define QCEF_BROWSER_TAB_WIDGET_H

#include <QTabWidget>
#include <QUrl>

struct BrowserTabWidgetPrivate;

class BrowserTabWidget : public QTabWidget {
  Q_OBJECT

 public:
  explicit BrowserTabWidget(QWidget* parent = nullptr);
  ~BrowserTabWidget() override;

 public slots:
  void createNewBrowser(bool in_background, const QUrl& url = QUrl());

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 private:
  BrowserTabWidgetPrivate* p_ = nullptr;

 private slots:
  void onTabCloseRequested(int index);
};


#endif //CEF_BROWSER_TAB_WIDGET_H
