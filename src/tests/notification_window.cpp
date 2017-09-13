// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "tests/notification_window.h"

#include <QPushButton>

#include "widgets/qcef_notification_service.h"

struct NotificationWindowPrivate {
  QCefNotificationService* service = nullptr;
  QPushButton* show_button = nullptr;
};

NotificationWindow::NotificationWindow(QWidget* parent)
    : QFrame(parent),
      p_(new NotificationWindowPrivate()) {
  p_->service = new QCefNotificationService(this);
  p_->show_button = new QPushButton("Show notification", this);
  connect(p_->show_button, &QPushButton::clicked, [=]() {
    p_->service->notify("Hello", "Body", QIcon());
  });
}

NotificationWindow::~NotificationWindow() {
  if (p_ != nullptr) {
    delete p_;
    p_ = nullptr;
  }
}
