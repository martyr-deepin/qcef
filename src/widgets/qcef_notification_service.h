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

#ifndef QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H
#define QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H

#include <QtDBus/QtDBus>

#include "qcef_widgets_export.h"

/**
 * Binding of freedesktop notifications dbus service.
 */
class QCEF_WIDGETS_EXPORT QCefNotificationService
    : public QDBusAbstractInterface {
  Q_OBJECT
 public:
  explicit QCefNotificationService(QObject* parent = nullptr);

 public slots:
  /**
   * Popup a new notification.
   * @param title Notification title.
   * @param body Notification content.
   * @param icon Notification icon.
   */
  void notify(const QString& title, const QString& body, const QIcon& icon);
};

#endif  // QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H
