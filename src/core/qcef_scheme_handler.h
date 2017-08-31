// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_SCHEME_HANDLER_H
#define QCEF_CORE_QCEF_SCHEME_HANDLER_H

#include <QUrl>

// User may implement this function to handle url requests with custom scheme
// names.
// Note that qrc: and file:/ have already been handled properly.
typedef QString (*QCefSchemeHandler)(const QUrl& url);

#endif  // QCEF_CORE_QCEF_SCHEME_HANDLER_H
