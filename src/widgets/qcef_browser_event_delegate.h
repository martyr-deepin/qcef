// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H
#define QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H

#include <X11/Xlib.h>

#include "qcef_widgets_export.h"

// Implement this class to handle mouse/keyboard events in cef browser window.
class QCEF_WIDGETS_EXPORT QCefBrowserEventDelegate {
 public:
  virtual bool onPreKeyEvent(XEvent* event) = 0;
};


#endif  // QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H
