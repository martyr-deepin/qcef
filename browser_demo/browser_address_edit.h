// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
