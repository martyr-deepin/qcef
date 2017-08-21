// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_URL_SCHEME_HANDLER_H
#define QCEF_CORE_QCEF_URL_SCHEME_HANDLER_H

#include <QString>
#include <QStringList>

#include "core/qcef_core_export.h"

// Inherit this class to handle request with custom scheme name.
class QCEF_CORE_EXPORT QCefUrlSchemeHandler {
 public:
  virtual const QString& getScheme() const = 0;
  virtual const QStringList& getHostList() const = 0;
  virtual const QString& getResourcePath(const QString& url) = 0;
};

#endif  // QCEF_CORE_QCEF_URL_SCHEME_HANDLER_H
