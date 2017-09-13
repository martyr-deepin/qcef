// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_TESTS_NOTIFICATION_WINDOW_H
#define QCEF_TESTS_NOTIFICATION_WINDOW_H

#include <QFrame>

struct NotificationWindowPrivate;

class NotificationWindow : public QFrame {
  Q_OBJECT
 public:
  explicit NotificationWindow(QWidget* parent = nullptr);
  ~NotificationWindow() override;

 private:
  NotificationWindowPrivate* p_ = nullptr;
};


#endif  // QCEF_TESTS_NOTIFICATION_WINDOW_H
