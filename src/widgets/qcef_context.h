// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_CONTEXT_H
#define QCEF_CORE_QCEF_CONTEXT_H

#include "qcef_widgets_export.h"
#include "qcef_global_settings.h"

// Attach CEF event worker to Qt Message loop.
// Call this function after QApplication object has been initialized.
// TODO(LiuLang): Remove this type of message loop binding. It takes more
// CPU time to pump CEF message workers into Qt message loop.
// DEPRECATED.
QCEF_WIDGETS_EXPORT bool AttachToQtMessageLoop();

// Initialize cef app with specific settings.
QCEF_WIDGETS_EXPORT int QCefInit(int argc,
                                 char* argv[],
                                 const QCefGlobalSettings& settings);

// Run cef message loop when every thing is ready.
QCEF_WIDGETS_EXPORT void QCefRunLoop();

// Quit cef message loop. Call this method when, like main window of app is
// closed or "Quit" menu item is activated.
QCEF_WIDGETS_EXPORT void QCefQuitLoop();

#endif  // QCEF_CORE_QCEF_CONTEXT_H
