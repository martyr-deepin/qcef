// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_CONTEXT_H
#define QCEF_CORE_QCEF_CONTEXT_H

#include "qcef_core_export.h"
#include "qcef_global_settings.h"

// Initialize cef app with specific settings.
QCEF_CORE_EXPORT int QCefInit(int argc,
                              char* argv[],
                              const QCefGlobalSettings& settings);

// Run cef message loop when every thing is ready.
QCEF_CORE_EXPORT void QCefRunLoop();

// Quit cef message loop. Call this method when, like main window of app is
// closed or "Quit" menu item is activated.
QCEF_CORE_EXPORT void QCefQuitLoop();

#endif  // QCEF_CORE_QCEF_CONTEXT_H
