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
