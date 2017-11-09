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

#include "browser_tab_widget.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QMouseEvent>
#include <QWebChannel>
#include <qcef_notification_service.h>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include "browser_event_delegate.h"
#include "browser_tab_bar.h"
#include "channel.h"

struct BrowserTabWidgetPrivate {
  BrowserEventDelegate* event_delegate = nullptr;
  BrowserTabBar* tab_bar = nullptr;
  QCefWebView* current_web = nullptr;
  QIcon blank_icon{":/images/document-new-symbolic.svg"};
  QCefNotificationService* notification_service = nullptr;

  Channel* channel = nullptr;
};

BrowserTabWidget::BrowserTabWidget(QWidget* parent)
    : QTabWidget(parent),
      p_(new BrowserTabWidgetPrivate()) {
  p_->event_delegate = new BrowserEventDelegate(this);
  p_->notification_service = new QCefNotificationService(this);

  p_->channel = new Channel(this);
  p_->tab_bar = new BrowserTabBar(this);
  this->setTabBar(p_->tab_bar);

  this->setTabsClosable(true);
  this->setMovable(true);
  this->setContentsMargins(0, 0, 0, 0);

  this->initConnections();
}

BrowserTabWidget::~BrowserTabWidget() {
  if (p_ != nullptr) {
    delete p_;
    p_ = nullptr;
  }
}

bool BrowserTabWidget::isLoading() const {
  return p_->current_web->page()->isLoading();
}

QCefSSLStatus BrowserTabWidget::getSSlStatus() const {
  return p_->current_web->page()->getSSLStatus();
}

void BrowserTabWidget::createNewBrowser(bool in_background,
                                        const QUrl& url) {
  QCefWebView* web_view = new QCefWebView();
  web_view->page()->setEventDelegate(p_->event_delegate);

  QCefWebPage* page = web_view->page();
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
  connect(page, &QCefWebPage::notificationReceived,
          this, &BrowserTabWidget::onNotificationReceived);
  connect(page, &QCefWebPage::fullscreenRequested,
          this, &BrowserTabWidget::fullscreenRequested);
  connect(page, &QCefWebPage::windowClosed, [=]() {
    const int index = this->indexOf(web_view);
    this->removeTab(index);
    web_view->deleteLater();
  });

  page->webChannel()->registerObject("channel", p_->channel);

  this->addTab(web_view, "New Tab");
  if (!in_background) {
    this->setCurrentWidget(web_view);
  }

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

void BrowserTabWidget::initConnections() {
  connect(this, &BrowserTabWidget::currentChanged,
          this, &BrowserTabWidget::onCurrentChanged);
  connect(this, &BrowserTabWidget::tabCloseRequested,
          this, &BrowserTabWidget::onTabCloseRequested);
  connect(this, &BrowserTabWidget::fullscreenRequested,
          this, &BrowserTabWidget::onFullscreenRequested);

  connect(p_->event_delegate, &BrowserEventDelegate::refreshRequested,
          this, &BrowserTabWidget::onRefreshRequested);
  connect(p_->event_delegate, &BrowserEventDelegate::popupRequested,
          this, &BrowserTabWidget::onPopupRequested);
  connect(p_->event_delegate, &BrowserEventDelegate::copyLinkToClipboard,
          this, &BrowserTabWidget::setClipboardUrl);
  connect(p_->event_delegate, &BrowserEventDelegate::toggleFullscreen,
          this, &BrowserTabWidget::onToggleFullscreen);
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

void BrowserTabWidget::onNotificationReceived(const QString& summary,
                                              const QString& body) {
  // TODO(LiuLang): popup notification only if current window is not activated.
  qDebug() << "show notification:" << summary << body;
  p_->notification_service->notify(summary, body, qApp->windowIcon());
}

void BrowserTabWidget::onTabCloseRequested(int index) {
  auto widget = this->widget(index);
  QCefWebView* web_view = qobject_cast<QCefWebView*>(widget);
  web_view->deleteLater();
  this->removeTab(index);
}

void BrowserTabWidget::onRefreshRequested() {
  p_->current_web->page()->reload();
}

void BrowserTabWidget::onPopupRequested(const QUrl& url,
                                        QCefWindowOpenDisposition disposition) {
  switch (disposition) {
    case QCEF_WOD_NEW_BACKGROUND_TAB: {
      this->createNewBrowser(true, url);
      break;
    }
    case QCEF_WOD_NEW_FOREGROUND_TAB:  // fall through
    case QCEF_WOD_NEW_WINDOW:  // fall through
    case QCEF_WOD_NEW_POPUP: {
      this->createNewBrowser(false, url);
      break;
    }
    case QCEF_WOD_SAVE_TO_DISK: {
      qDebug() << "save file to disk:" << url;
      break;
    }
    default: {
      break;
    }
  }
}

void BrowserTabWidget::onToggleFullscreen() {
//  p_->tab_bar->setVisible(!p_->tab_bar->isVisible());
}

void BrowserTabWidget::setClipboardUrl(const QUrl& url) {
  QApplication::clipboard()->setText(url.toString());
}
