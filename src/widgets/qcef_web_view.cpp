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

#include "widgets/qcef_web_view.h"

#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QTimer>

#include "widgets/qcef_web_page.h"

namespace {

const int kMoveEventInterval = 500;

}  // namespace

struct QCefWebViewPrivate {
  QCefWebPage* page = nullptr;
  QTimer* move_event_timer = nullptr;
};

QCefWebView::QCefWebView(QWidget* parent)
    : QWidget(parent),
      p_(new QCefWebViewPrivate()) {

  // Child window will fill this widget.
  QStackedLayout* layout = new QStackedLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);

  p_->move_event_timer = new QTimer();
  p_->move_event_timer->setSingleShot(true);
//  connect(p_->move_event_timer, &QTimer::timeout, [this]() {
//    if (this->p_->page != nullptr) {
//      this->p_->page->updateBrowserGeometry();
//    }
//  });
//  qApp->installEventFilter(this);
}

QCefWebView::~QCefWebView() {
  qDebug() << Q_FUNC_INFO;
  if (p_ != nullptr) {
    if (p_->move_event_timer != nullptr) {
      p_->move_event_timer->stop();
      delete p_->move_event_timer;
      p_->move_event_timer = nullptr;
    }

    if (p_->page != nullptr) {
      delete p_->page;
      p_->page = nullptr;
    }

    delete p_;
    p_ = nullptr;
  }
}

void QCefWebView::load(const QUrl& url) {
  this->page()->load(url);
}

void QCefWebView::setUrl(const QUrl& url) {
  this->page()->setUrl(url);
}

QUrl QCefWebView::url() const {
  return this->page()->url();
}

QCefWebPage* QCefWebView::page() const {
  if (p_->page == nullptr) {
    QCefWebView* that = const_cast<QCefWebView*>(this);
    that->p_->page = new QCefWebPage(that);
  }
  return p_->page;
}

bool QCefWebView::eventFilter(QObject* watched, QEvent* event) {
  if (event->type() == QEvent::Move) {
    p_->move_event_timer->start(kMoveEventInterval);
  }
  return QObject::eventFilter(watched, event);
}

void QCefWebView::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  QTimer::singleShot(1, [=]() {
    page()->remapBrowserWindow(this->winId());
  });
}

void QCefWebView::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  page()->updateBrowserGeometry(event->size());
}
