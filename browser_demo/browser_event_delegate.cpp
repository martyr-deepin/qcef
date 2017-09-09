// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "browser_event_delegate.h"

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
