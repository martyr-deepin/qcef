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

#include "core/qcef_message_pump_handler.h"

#include <QCoreApplication>

#include "include/cef_app.h"

QCefMessagePumpHandler::QCefMessagePumpHandler(QObject* parent)
    : QObject(parent), timer_id_(0) {
}

void QCefMessagePumpHandler::scheduleWork(qint64 delayed_ms) {
  if (delayed_ms <= 0) {
    QCoreApplication::postEvent(this, new QEvent(QEvent::User));
  } else {
    killTimer(timer_id_);
    // Limit delayed time to 50ms.
    const int delay = qMin(static_cast<int>(delayed_ms), 50);
    timer_id_ = startTimer(delay);
  }
}

void QCefMessagePumpHandler::timerEvent(QTimerEvent* event) {
  Q_ASSERT(event->timerId() == timer_id_);
  QObject::timerEvent(event);
  CefDoMessageLoopWork();
}

void QCefMessagePumpHandler::customEvent(QEvent* event) {
  QObject::customEvent(event);
  CefDoMessageLoopWork();
}
