// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_QCEF_KEY_EVENT_MAP_H
#define QCEF_QCEF_KEY_EVENT_MAP_H

#include <QKeyEvent>

#include "include/internal/cef_linux.h"

// Map XEvent(XKeyEvent) to QKeyEvent
QKeyEvent XEvent2QtKeyEvent(CefEventHandle event);

#endif //QCEF_QCEF_KEY_EVENT_MAP_H
