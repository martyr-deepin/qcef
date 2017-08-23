// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_web_view.h"

#include <QDebug>

#include "widgets/qcef_web_page.h"
#include "widgets/qcef_web_settings.h"

struct QCefWebViewPrivate {
  QCefWebPage* page = nullptr;
};

QCefWebView::QCefWebView(QWidget* parent)
    : QCefWebView(nullptr, parent) {
}

QCefWebView::QCefWebView(QCefWebSettings* settings, QWidget* parent)
    : QWidget(parent),
      p_(new QCefWebViewPrivate()) {
  this->setAttribute(Qt::WA_NativeWindow, true);
  this->setAttribute(Qt::WA_DontCreateNativeAncestors, true);

  p_->page = new QCefWebPage(settings, this);
  p_->page->createBrowser(this->windowHandle(), this->size());
}

QCefWebView::~QCefWebView() {
  delete p_;
  p_ = nullptr;
}

void QCefWebView::load(const QUrl& url) {
  p_->page->load(url);
}

void QCefWebView::setUrl(const QUrl& url) {
  p_->page->setUrl(url);
}

QUrl QCefWebView::url() const {
  return p_->page->url();
}

QCefWebPage* QCefWebView::page() const {
  return p_->page;
}

void QCefWebView::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  if (p_->page != nullptr) {
    // Update geometry of inner windows.
    p_->page->resizeCefBrowser(this->size());
  }
}
