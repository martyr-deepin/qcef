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

#ifndef QCEF_WIDGETS_QCEF_SSL_STATUS_H
#define QCEF_WIDGETS_QCEF_SSL_STATUS_H

#include "qcef_widgets_export.h"

/**
 * Supported SSL content status flags. See content/public/common/ssl_status.h
 * for more information.
 */
enum QCEF_WIDGETS_EXPORT QCefSSLContentStatus {
  NORMAL_CONTENT = 0,
  DISPLAYED_INSECURE_CONTENT = 1 << 0,
  RAN_INSECURE_CONTENT = 1 << 1,
};

struct QCEF_WIDGETS_EXPORT QCefSSLStatus {
  /**
   * Whether the status is related to a secure SSL/TLS connection.
   */
  bool is_secure_connection = false;

  /**
   * A bit mask containing the page security content status.
   */
  QCefSSLContentStatus content_status = QCefSSLContentStatus::NORMAL_CONTENT;
};

#endif  // QCEF_WIDGETS_QCEF_SSL_STATUS_H
