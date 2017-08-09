// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_browser_transport.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/qcef_web_channel_consts.h"

QCefBrowserTransport::QCefBrowserTransport(CefRefPtr<CefBrowser> browser,
                                           QObject* parent)
    : QWebChannelAbstractTransport(parent),
      browser_(browser) {
}

void QCefBrowserTransport::sendMessage(const QJsonObject& message) {
  // Send message from browser process to renderer process.
  CefRefPtr<CefProcessMessage> msg =
      CefProcessMessage::Create(kQCefRenderQtMessage);
  CefRefPtr<CefListValue> args = msg->GetArgumentList();
  const std::string data = QJsonDocument(message).toJson().toStdString();
  args->SetString(0, data);
  browser_->SendProcessMessage(PID_RENDERER, msg);
}
