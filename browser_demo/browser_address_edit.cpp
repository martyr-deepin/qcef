// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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