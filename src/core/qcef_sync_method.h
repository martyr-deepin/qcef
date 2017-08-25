// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_SYNC_METHOD_H
#define QCEF_CORE_QCEF_SYNC_METHOD_H

#include <QMap>
#include <QString>

// Register sync methods in global context of V8 engine.
// Note that this method is called in render process.
// If multiple arguments or return values need to be handled, try to serialize
// them.
typedef QString (*QCefSyncMethod)(const QString& argument);
typedef QMap<QString, QCefSyncMethod> QCefSyncMethodMap;

#endif  // QCEF_CORE_QCEF_SYNC_METHOD_H
