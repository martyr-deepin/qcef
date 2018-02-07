/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

/**
 * Initialize cef app with specific settings.
 * @param argc Number of arguments.
 * @param argv Pointer to argument list.
 * @param settings Reference to global settings.
 * @return 0 on success of browser process, > 0 on success of zygto processes, < 0 on error.
 */
QCEF_CORE_EXPORT int QCefInit(int argc,
                              char** argv,
                              const QCefGlobalSettings& settings);

/**
 * Bind cef message loop to Qt main message loop.
 * @param app Initialized Qt QApplication instance.
 */
QCEF_CORE_EXPORT void QCefBindApp(QCoreApplication* app);

/**
 * Stop internal timer explicitly.
 * Normally no need to call this method to stop cef worker.
 */
QCEF_CORE_EXPORT void QCefStopTimer();

/**
 * Stop renderer processes.
 * Normally no need to call this method to cleanup sub-processes.
 */
QCEF_CORE_EXPORT void QCefShutdown();

#endif  // QCEF_CORE_QCEF_CONTEXT_H
