// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "browser_event_delegate.h"

#include <QDebug>
#include <QKeyEvent>

BrowserEventDelegate::BrowserEventDelegate(QObject* parent) : QObject(parent) {

}

bool BrowserEventDelegate::onBeforePopup(
    const QUrl& url,
    QCefWindowOpenDisposition disposition) {
  emit this->popupRequested(url, disposition);
  return true;
}

bool BrowserEventDelegate::onPreKeyEvent(const QKeyEvent& event) {
  Q_UNUSED(event);
  return false;
}

bool BrowserEventDelegate::onBeforeBrowse(const QUrl& url, bool is_redirect) {
  if (url.host() == "bing.com") {
    qDebug() << "Prevent browser from navigating to bing.com, " << is_redirect;
    return true;
  }
  return false;
}
