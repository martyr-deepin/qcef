/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#ifndef QCEF_WIDGETS_QCEF_NATIVE_EVENT_FILTER_H
#define QCEF_WIDGETS_QCEF_NATIVE_EVENT_FILTER_H

#include <QAbstractNativeEventFilter>

class QCefNativeEventFilter : public QAbstractNativeEventFilter {
 public:
  // Install global instance of event filter.
  static void install();

  bool nativeEventFilter(const QByteArray& event_type,
                         void* message,
                         long* result) override;

 private:
  QCefNativeEventFilter() { }
};

#endif  // QCEF_WIDGETS_QCEF_NATIVE_EVENT_FILTER_H
