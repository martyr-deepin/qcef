// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H
#define QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H

#include <QtDBus>

#include "qcef_widgets_export.h"

// Binding of freedesktop notifications dbus service.
class QCEF_WIDGETS_EXPORT QCefNotificationService
    : public QDBusAbstractInterface {
  Q_OBJECT
 public:
  explicit QCefNotificationService(QObject* parent = nullptr);

 public slots:
  // Popup a new notification.
  void notify(const QString& title, const QString& body, const QIcon& icon);
};

#endif  // QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H
