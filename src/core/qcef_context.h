// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
