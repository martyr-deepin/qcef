// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_BROWSER_EVENT_DELEGATE_H
#define QCEF_BROWSER_EVENT_DELEGATE_H

#include <QObject>
#include <qcef_browser_event_delegate.h>

struct BrowserEventDelegatePrivate;

class BrowserEventDelegate : public QObject,
                             public QCefBrowserEventDelegate {
 Q_OBJECT

 public:
  explicit BrowserEventDelegate(QObject* parent = nullptr);

  ~BrowserEventDelegate() override;

  bool onBeforeBrowse(const QUrl& url, bool is_redirect) override;

  void onBeforeContextMenu(QCefWebPage* web_page,
                           QCefContextMenu* menu,
                           const QCefContextMenuParams& params) override;

  bool onBeforePopup(const QUrl& url,
                     QCefWindowOpenDisposition disposition) override;

  bool onPreKeyEvent(const QKeyEvent& event) override;

 signals:
  void popupRequested(const QUrl& url, QCefWindowOpenDisposition disposition);
  void copyLinkToClipboard(const QUrl& url);

 private:
  BrowserEventDelegatePrivate* p_ = nullptr;
};

#endif  // QCEF_BROWSER_EVENT_DELEGATE_H
