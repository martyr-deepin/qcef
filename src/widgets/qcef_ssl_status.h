// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_SSL_STATUS_H
#define QCEF_WIDGETS_QCEF_SSL_STATUS_H

#include "qcef_widgets_export.h"

// Supported SSL content status flags. See content/public/common/ssl_status.h
// for more information.
enum class QCEF_WIDGETS_EXPORT QCefSSLContentStatus {
  NORMAL_CONTENT = 0,
  DISPLAYED_INSECURE_CONTENT = 1 << 0,
  RAN_INSECURE_CONTENT = 1 << 1,
};

struct QCEF_WIDGETS_EXPORT QCefSSLStatus {
  // Whether the status is related to a secure SSL/TLS connection.
  bool is_secure_connection = false;

  // A bit mask containing the page security content status.
  QCefSSLContentStatus content_status = QCefSSLContentStatus::NORMAL_CONTENT;
};

#endif  // QCEF_WIDGETS_QCEF_SSL_STATUS_H
