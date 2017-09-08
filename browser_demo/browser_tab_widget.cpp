// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "browser_tab_widget.h"

#include <QDebug>
#include <QMouseEvent>
#include <qcef_web_page.h>
#include <qcef_web_view.h>

#include "browser_tab_bar.h"

struct BrowserTabWidgetPrivate {
  QCefWebView* current_web = nullptr;
};

BrowserTabWidget::BrowserTabWidget(QWidget* parent)
    : QTabWidget(parent),
      p_(new BrowserTabWidgetPrivate()) {
  BrowserTabBar* tab_bar = new BrowserTabBar(this);
  this->setTabBar(tab_bar);

  this->setTabsClosable(true);
  this->setMovable(true);

  connect(this, &BrowserTabWidget::tabCloseRequested,
          this, &BrowserTabWidget::onTabCloseRequested);
}

BrowserTabWidget::~BrowserTabWidget() {
  if (p_ != nullptr) {
    delete p_;
    p_ = nullptr;
  }
}

void BrowserTabWidget::createNewBrowser(bool in_background,
                                        const QUrl& url) {
  qDebug() << "create new browser";
  auto web_view = new QCefWebView();
  this->addTab(web_view, "New Tab");
  if (!in_background) {
    this->setCurrentWidget(web_view);
  }
  if (url.isValid()) {
    web_view->load(url);
  }
}

void BrowserTabWidget::mouseDoubleClickEvent(QMouseEvent* event) {
  QWidget::mouseDoubleClickEvent(event);

  // Create new browser if double clicked on blank area.
  if (this->childAt(event->pos()) == nullptr) {
    this->createNewBrowser(false);
  }
}

void BrowserTabWidget::onTabCloseRequested(int index) {
  if (this->count() == 1) {
    // If only one web view is displayed, do nothing.
    return;
  }
  auto web_view = qobject_cast<QCefWebView*>(this->widget(index));
  web_view->deleteLater();
  this->removeTab(index);
}
