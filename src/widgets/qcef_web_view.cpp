// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
}

QCefWebView::~QCefWebView() {
  delete p_;
  p_ = nullptr;
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