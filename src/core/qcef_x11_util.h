// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_X11_UTIL_H
#define QCEF_CORE_QCEF_X11_UTIL_H

#include "include/internal/cef_linux.h"

void SetXErrorHandler();

unsigned long InitCefBrowserWindow(int width, int height);

void SetXWindowBounds(CefWindowHandle xwindow,
                      int x, int y,
                      int width, int height);

void SetXWindowTitle(CefWindowHandle window, const std::string& title);

void SetXWindowVisible(CefWindowHandle xwindow, bool visible);

#endif  // QCEF_CORE_QCEF_X11_UTIL_H
