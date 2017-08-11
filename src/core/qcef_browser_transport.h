// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_WEB_CHANNEL_TRANSPORT_H
#define QCEF_CORE_QCEF_WEB_CHANNEL_TRANSPORT_H

#include <QWebChannelAbstractTransport>

#include "include/cef_browser.h"
#include "include/internal/cef_ptr.h"

// Communication channel between browser process and renderer process.
class QCefBrowserTransport : public QWebChannelAbstractTransport {
  Q_OBJECT

 public:
  QCefBrowserTransport(CefRefPtr<CefBrowser> browser,
                       QObject* parent = nullptr);

  // Send messages from between browser process to render process.
  void sendMessage(const QJsonObject& message) override;

 private:
  CefRefPtr<CefBrowser> browser_ = nullptr;
};


#endif  // QCEF_CORE_QCEF_WEB_CHANNEL_TRANSPORT_H
