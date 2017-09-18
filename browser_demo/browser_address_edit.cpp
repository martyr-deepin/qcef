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

#include "browser_address_edit.h"

namespace {

const char kLockIcon[] = ":/images/system-lock-screen-symbolic.svg";

} // namespace

BrowserAddressEdit::BrowserAddressEdit(QWidget* parent) : QLineEdit(parent) {
  this->setStyleSheet("QLineEdit {padding-left: 26px; margin: 0px}");
  ssl_button_ = new QPushButton(this);
  ssl_button_->setIcon(QIcon(kLockIcon));
  ssl_button_->move(2, 0);
  ssl_button_->setFixedSize(24, 24);
  ssl_button_->setFlat(true);
  ssl_button_->hide();
}

void BrowserAddressEdit::updateCertificatesInfo(bool secure) {
  if (secure) {
    ssl_button_->show();
  } else {
    ssl_button_->hide();
  }
}