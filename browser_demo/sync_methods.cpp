// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sync_methods.h"

#include <QDebug>

QString EchoMessage(const QString& msg) {
  qDebug() << __FUNCTION__ << msg;
  return msg;
}