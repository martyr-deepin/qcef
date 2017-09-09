// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_BROWSER_EVENT_DELEGATE_H
#define QCEF_BROWSER_EVENT_DELEGATE_H

#include <QObject>
#include <qcef_browser_event_delegate.h>

class BrowserEventDelegate : public QObject,
                             public QCefBrowserEventDelegate {
  Q_OBJECT

 public:
  explicit BrowserEventDelegate(QObject* parent = nullptr);

  bool onPreKeyEvent(const QKeyEvent& event) override;

  bool onBeforePopup(const QUrl& url,
                     QCefWindowOpenDisposition disposition) override;

 signals:
  void popupRequested(const QUrl& url, QCefWindowOpenDisposition disposition);
};

#endif  // QCEF_BROWSER_EVENT_DELEGATE_H
