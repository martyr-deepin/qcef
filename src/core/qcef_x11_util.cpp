// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_x11_util.h"

#include <X11/Xlib.h>

#include "include/base/cef_logging.h"

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