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
  BrowserTabBar* tab_bar = nullptr;
  QCefWebView* current_web = nullptr;
  QIcon blank_icon{":/images/document-new-symbolic.svg"};
};

BrowserTabWidget::BrowserTabWidget(QWidget* parent)
    : QTabWidget(parent),
      p_(new BrowserTabWidgetPrivate()) {
  p_->tab_bar = new BrowserTabBar(this);
  this->setTabBar(p_->tab_bar);

  this->setTabsClosable(true);
  this->setMovable(true);
  this->setContentsMargins(0, 0, 0, 0);

  connect(this, &BrowserTabWidget::currentChanged,
          this, &BrowserTabWidget::onCurrentChanged);
  connect(this, &BrowserTabWidget::tabCloseRequested,
          this, &BrowserTabWidget::onTabCloseRequested);
  connect(this, &BrowserTabWidget::fullscreenRequested,
          this, &BrowserTabWidget::onFullscreenRequested);
}

BrowserTabWidget::~BrowserTabWidget() {
  if (p_ != nullptr) {
    delete p_;
    p_ = nullptr;
  }
}

bool BrowserTabWidget::isLoading() const {
  p_->current_web->page()->isLoading();
}

QCefSSLStatus BrowserTabWidget::getSSlStatus() const {
  return p_->current_web->page()->getSSLStatus();
}

void BrowserTabWidget::createNewBrowser(bool in_background,
                                        const QUrl& url) {
  qDebug() << "create new browser";
  auto web_view = new QCefWebView();
  this->addTab(web_view, "New Tab");
  if (!in_background) {
    this->setCurrentWidget(web_view);
  }
  auto page = web_view->page();
  connect(page, &QCefWebPage::iconChanged,
          [this, web_view](const QIcon& icon) {
            const int index = this->indexOf(web_view);
            this->setTabIcon(index, icon);
          });
  connect(page, &QCefWebPage::loadStarted,
          [this, web_view]() {
            const int index = this->indexOf(web_view);
            // Set loading animation icon.
            this->setTabIcon(index, p_->blank_icon);
          });
  connect(page, &QCefWebPage::titleChanged,
          [this, web_view](const QString& title) {
            const int index = this->indexOf(web_view);
            this->setTabText(index, title);
            this->setTabToolTip(index, title);
          });
  connect(page, &QCefWebPage::fullscreenRequested,
          this, &BrowserTabWidget::fullscreenRequested);
  if (url.isValid()) {
    web_view->load(url);
  }
}

void BrowserTabWidget::back() {
  p_->current_web->page()->back();
}

void BrowserTabWidget::forward() {
  p_->current_web->page()->forward();
}

void BrowserTabWidget::reload() {
  p_->current_web->page()->reload();
}

void BrowserTabWidget::stop() {
  p_->current_web->page()->stop();
}

void BrowserTabWidget::load(const QUrl& url) {
  p_->current_web->load(url);
}

void BrowserTabWidget::zoomIn() {
  p_->current_web->page()->zoomIn();
}

void BrowserTabWidget::zoomOut() {
  p_->current_web->page()->zoomOut();
}

void BrowserTabWidget::zoomReset() {
  p_->current_web->page()->resetZoomFactor();
}

void BrowserTabWidget::mouseDoubleClickEvent(QMouseEvent* event) {
  QWidget::mouseDoubleClickEvent(event);

  // Create new browser if double clicked on blank area.
  if (this->childAt(event->pos()) == nullptr) {
    this->createNewBrowser(false);
  }
}

void BrowserTabWidget::onCurrentChanged(int index) {
  qDebug() << "on current changed:" << index;
  QCefWebView* web_view = qobject_cast<QCefWebView*>(this->widget(index));
  if (p_->current_web != nullptr) {
    // Disconnect signals of old web view.
    p_->current_web->disconnect(this);
    auto old_page = p_->current_web->page();
    old_page->disconnect(this, SIGNAL(urlChanged(QUrl)));
    old_page->disconnect(this, SIGNAL(loadingStateChanged(bool, bool, bool)));
  }
  p_->current_web = web_view;
  // Connect signals.
  auto page = web_view->page();
  connect(page, &QCefWebPage::urlChanged,
          this, &BrowserTabWidget::urlChanged);
  connect(page, &QCefWebPage::loadingStateChanged,
          this, &BrowserTabWidget::loadingStateChanged);

  emit this->loadingStateChanged(page->isLoading(),
                                 page->canGoBack(),
                                 page->canGoForward());
  emit this->urlChanged(page->url());
}

void BrowserTabWidget::onFullscreenRequested(bool fullscreen) {
  p_->tab_bar->setVisible(!fullscreen);
}

void BrowserTabWidget::onTabCloseRequested(int index) {
  if (this->count() == 1) {
    // If only one web view is displayed, do nothing.
    return;
  }
  auto web_view = qobject_cast<QCefWebView*>(this->widget(index));
  delete web_view;
  this->removeTab(index);
}
