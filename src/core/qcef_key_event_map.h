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

#ifndef QCEF_QCEF_KEY_EVENT_MAP_H
#define QCEF_QCEF_KEY_EVENT_MAP_H

#include <QKeyEvent>

#include "include/internal/cef_linux.h"

// Map XEvent(XKeyEvent) to QKeyEvent
QKeyEvent XEvent2QtKeyEvent(CefEventHandle event);

#endif //QCEF_QCEF_KEY_EVENT_MAP_H
