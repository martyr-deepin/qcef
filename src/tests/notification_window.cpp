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
