// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_web_view.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>

#include "widgets/qcef_web_page.h"
#include "widgets/qcef_web_settings.h"

struct QCefWebViewPrivate {
  QCefWebPage* page = nullptr;
};

QCefWebView::QCefWebView(QWidget* parent)
    : QWidget(parent),
      p_(new QCefWebViewPrivate()) {
  this->setAttribute(Qt::WA_NativeWindow, true);
  this->setAttribute(Qt::WA_DontCreateNativeAncestors, true);
  this->setFocusPolicy(Qt::StrongFocus);

  qApp->installEventFilter(this);

  p_->page = new QCefWebPage(this);
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

bool QCefWebView::eventFilter(QObject* watched, QEvent* event) {
  if (event->type() == QEvent::Move) {
    if (p_->page != nullptr) {
      p_->page->updateBrowserWindowGeometry();
    }
  }
  return QObject::eventFilter(watched, event);
}
