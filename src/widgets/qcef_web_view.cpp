/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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
#include <QTimer>
#include <QWindow>

#include <cmath>

#include "widgets/qcef_web_page.h"

struct QCefWebViewPrivate {
  QCefWebPage* page = nullptr;
  bool window_mapped = false;
  bool auto_zoom = true;
};

QCefWebView::QCefWebView(QWidget* parent)
    : QWidget(parent),
      p_(new QCefWebViewPrivate()) {
  this->setAttribute(Qt::WA_NativeWindow, true);
  this->setAttribute(Qt::WA_DontCreateNativeAncestors, true);

  connect(page(), &QCefWebPage::renderContextCreated,
          this, &QCefWebView::updateWebZoom, Qt::QueuedConnection);

  if (qApp->metaObject()->indexOfSignal("screenDevicePixelRatioChanged(QScreen*)") >= 0) {
    connect(qApp, SIGNAL(screenDevicePixelRatioChanged(QScreen*)),
            this, SLOT(onScreenScaleChanged(QScreen*)), Qt::QueuedConnection);
  }
}

QCefWebView::~QCefWebView() {
  if (p_ != nullptr) {

    if (p_->page != nullptr) {
      p_->page->deleteLater();
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

bool QCefWebView::autoZoom() const
{
  return p_->auto_zoom;
}

void QCefWebView::setAutoZoom(bool autoZoom)
{
  if (p_->auto_zoom == autoZoom)
    return;

  p_->auto_zoom = autoZoom;

  updateWebZoom();
}

void QCefWebView::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  if (!p_->window_mapped) {
    p_->window_mapped = true;
    QTimer::singleShot(1, this, [=]() {
      page()->remapBrowserWindow(this->winId());
    });
    updateWebZoom();
  }
}

void QCefWebView::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  page()->updateBrowserGeometry(event->size());
}

void QCefWebView::focusInEvent(QFocusEvent *event)
{
  if (p_->page) {
    p_->page->setFocus(true);
  }

  QWidget::focusInEvent(event);
}

bool QCefWebView::event(QEvent *event)
{
  if (event->type() == QEvent::ScreenChangeInternal) {
    updateWebZoom();
  }

  return QWidget::event(event);
}

void QCefWebView::updateWebZoom()
{
  if (!p_->window_mapped)
    return;

  if (autoZoom()) {
    // chromium 中缩放比例以 20% 递增，因此，zoomLevel 的值实际上表示递增的倍数
    // 如，值为1时表示缩放为1.2倍，值为2时表示缩放为1.2*1.2倍
    page()->setZoomFactor(std::log(devicePixelRatioF()) / std::log(1.2));
  } else {
    page()->resetZoomFactor();
  }
}

void QCefWebView::onScreenScaleChanged(QScreen *screen)
{
  if (window()->windowHandle() && window()->windowHandle()->screen() == screen) {
    updateWebZoom();
  }
}
