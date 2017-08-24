// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_util.h"

#include <X11/Xlib.h>

#undef Success     // Definition conflicts with cef_message_router.h
#undef RootWindow  // Definition conflicts with root_window.h

#include "include/base/cef_logging.h"

namespace {

const char kProcessType[] = "type";
const char kRendererProcess[] = "renderer";
const char kZygoteProcess[] = "zygote";

}  // namespace

ProcessType GetProcessType(CefRefPtr<CefCommandLine> command_line) {
  if (!command_line->HasSwitch(kProcessType)) {
    return ProcessType::BrowserProcess;
  }

  const std::string& process_type = command_line->GetSwitchValue(kProcessType);
  if (process_type == kRendererProcess) {
    return ProcessType::RendererProcess;
  } else if (process_type == kZygoteProcess) {
    return ProcessType::ZygoteProcess;
  } else {
    return ProcessType::OtherProcess;
  }
}

int XErrorHandlerImpl(Display* display, XErrorEvent* event) {
  (void)display;

  LOG(WARNING)
  << "X error received: "
  << "type " << event->type << ", "
  << "serial " << event->serial << ", "
  << "error_code " << static_cast<int>(event->error_code) << ", "
  << "request_code " << static_cast<int>(event->request_code) << ", "
  << "minor_code " << static_cast<int>(event->minor_code);
  return 0;
}

int XIOErrorHandlerImpl(Display* display) {
  (void)display;
  return 0;
}

void SetXErrorHandler() {
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);
}