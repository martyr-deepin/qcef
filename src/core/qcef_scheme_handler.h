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

#ifndef QCEF_CORE_QCEF_SCHEME_HANDLER_H
#define QCEF_CORE_QCEF_SCHEME_HANDLER_H

#include <QUrl>

// User may implement this function to handle url requests with custom scheme
// names.
// Note that qrc: and file:/ have already been handled properly.
typedef QString (*QCefSchemeHandler)(const QUrl& url);

#endif  // QCEF_CORE_QCEF_SCHEME_HANDLER_H
