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

#ifndef QCEF_CORE_QCEF_MESSAGE_PUMP_HANDLER_H
#define QCEF_CORE_QCEF_MESSAGE_PUMP_HANDLER_H

#include <QObject>

// Integrate CEF Message with Qt Message Loop.
class QCefMessagePumpHandler : public QObject {
  Q_OBJECT
 public:
  explicit QCefMessagePumpHandler(QObject* parent = nullptr);

  void scheduleWork(qint64 delayed_ms);

 protected:
  void customEvent(QEvent* event) override;

  void timerEvent(QTimerEvent* event) override;

 private:
  int timer_id_;
};

#endif  // QCEF_CORE_QCEF_MESSAGE_PUMP_HANDLER_H
