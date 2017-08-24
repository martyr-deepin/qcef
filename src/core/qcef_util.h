// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_UTIL_H
#define QCEF_CORE_QCEF_UTIL_H

#include "include/cef_command_line.h"
#include "include/internal/cef_ptr.h"

enum class ProcessType {
  BrowserProcess,
  RendererProcess,
  ZygoteProcess,
  OtherProcess,
};

ProcessType GetProcessType(CefRefPtr<CefCommandLine> command_line);

void SetXErrorHandler();

#endif  // QCEF_CORE_QCEF_UTIL_H
