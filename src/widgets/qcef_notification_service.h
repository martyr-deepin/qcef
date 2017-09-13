// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H
#define QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H

#include <QtDBus>

class QCefNotificationService : public QDBusAbstractInterface {
  Q_OBJECT
 public:
  explicit QCefNotificationService(QObject* parent = nullptr);

 public slots:
  void notify(const QString& title, const QString& body, const QIcon& icon);
};

#endif  // QCEF_WIDGETS_QCEF_NOTIFICATION_SERVICE_H
