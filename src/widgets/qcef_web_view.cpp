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

#include <QDebug>
#include <QEvent>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QTimer>

#include "widgets/qcef_web_page.h"

struct QCefWebViewPrivate {
  QCefWebPage* page = nullptr;
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

  this->installEventFilter(this);
}

QCefWebView::~QCefWebView() {
  if (p_ != nullptr) {
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

bool QCefWebView::event(QEvent* event) {
  if (event->type() == QEvent::WindowActivate) {
    this->page()->updateBrowserGeometry();
  }
  return QWidget::event(event);
}
