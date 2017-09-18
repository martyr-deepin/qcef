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

#ifndef QCEF_BROWSER_ADDRESS_EDIT_H
#define QCEF_BROWSER_ADDRESS_EDIT_H

#include <QLineEdit>
#include <QPushButton>

class BrowserAddressEdit : public QLineEdit {
  Q_OBJECT
 public:
  explicit BrowserAddressEdit(QWidget* parent = nullptr);

  void updateCertificatesInfo(bool secure);

 private:
  QPushButton* ssl_button_ = nullptr;
};

#endif  // QCEF_BROWSER_ADDRESS_EDIT_H
