/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "core/qcef_browser_transport.h"

#include <string>
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
  qDebug() << "Browser sendMessage()" << message;
  CefRefPtr<CefProcessMessage> msg =
      CefProcessMessage::Create(kQCefRenderQtMessage);
  CefRefPtr<CefListValue> args = msg->GetArgumentList();
  const std::string data = QJsonDocument(message).toJson().toStdString();
  qDebug() << "data: " << data.c_str();
  args->SetString(0, data);
  browser_->SendProcessMessage(PID_RENDERER, msg);
}
