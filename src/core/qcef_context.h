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

#ifndef QCEF_CORE_QCEF_CONTEXT_H
#define QCEF_CORE_QCEF_CONTEXT_H

#include "qcef_core_export.h"
#include "qcef_global_settings.h"

class QCoreApplication;

// Initialize cef app with specific settings.
QCEF_CORE_EXPORT int QCefInit(int argc,
                              char** argv,
                              const QCefGlobalSettings& settings);

// Bind cef message loop to Qt main message loop.
QCEF_CORE_EXPORT void QCefBindApp(QCoreApplication* app);

// Run cef message loop when every thing is ready.
// Deprecated.
QCEF_CORE_EXPORT void QCefRunLoop();

// Quit cef message loop. Call this method when, like main window of app is
// closed or "Quit" menu item is activated.
// Deprecated.
QCEF_CORE_EXPORT void QCefQuitLoop();

#endif  // QCEF_CORE_QCEF_CONTEXT_H
