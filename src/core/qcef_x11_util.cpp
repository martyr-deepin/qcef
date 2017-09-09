// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_x11_util.h"

#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <X11/Xlib.h>

#undef Success     // Definition conflicts with cef_message_router.h
#undef RootWindow  // Definition conflicts with root_window.h

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

unsigned long InitCefBrowserWindow(int width, int height) {
  auto gtk_window = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_resize(GTK_WINDOW(gtk_window), width, height);
  gtk_widget_show_all(gtk_window);
  auto window_id = GDK_WINDOW_XID(gtk_widget_get_window(gtk_window));
  return window_id;
}

void SetXWindowBounds(CefWindowHandle xwindow, int x, int y,
                      int width, int height) {
  ::Display* xdisplay = cef_get_xdisplay();
  XWindowChanges changes;
  changes.x = x;
  changes.y = y;
  changes.width = width;
  changes.height = height;
  XConfigureWindow(xdisplay, xwindow,
                   CWX | CWY | CWHeight | CWWidth,
                   &changes);
}

void SetXWindowTitle(CefWindowHandle window, const std::string& title) {
  // Retrieve the X11 display shared with Chromium.
  ::Display* display = cef_get_xdisplay();
  DCHECK(display);

  // Retrieve the X11 window handle for the browser.
  DCHECK(window != kNullWindowHandle);

  // Retrieve the atoms required by the below XChangeProperty call.
  const char* kAtoms[] = {"_NET_WM_NAME", "UTF8_STRING"};
  Atom atoms[2];
  int result = XInternAtoms(display, const_cast<char**>(kAtoms), 2, 0,
                            atoms);
  if (result == 0) {
    NOTREACHED();
  }

  // Set the window title.
  XChangeProperty(display, window, atoms[0], atoms[1], 8, PropModeReplace,
                  reinterpret_cast<const unsigned char*>(title.c_str()),
                  static_cast<int>(title.size()));

  // TODO(erg): This is technically wrong. So XStoreName and friends expect
  // this in Host Portable Character Encoding instead of UTF-8, which I believe
  // is Compound Text. This shouldn't matter 90% of the time since this is the
  // fallback to the UTF8 property above.
  XStoreName(display, window, title.c_str());
}

void SetXWindowVisible(CefWindowHandle xwindow, bool visible) {
  ::Display* xdisplay = cef_get_xdisplay();

  // Retrieve the atoms required by the below XChangeProperty call.
  const char* kAtoms[] = {
      "_NET_WM_STATE",
      "ATOM",
      "_NET_WM_STATE_HIDDEN"
  };
  Atom atoms[3];
  int result = XInternAtoms(xdisplay, const_cast<char**>(kAtoms), 3, 0,
                            atoms);
  if (result == 0) {
    NOTREACHED();
  }

  if (!visible) {
    // Set the hidden property state value.
    Atom data[] = {atoms[2]};
    XChangeProperty(xdisplay,
                    xwindow,
                    atoms[0],  // name
                    atoms[1],  // type
                    32,  // size in bits of items in 'value'
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&data),
                    1);  // num items
  } else {
    // Set an empty array of property state values.
    XChangeProperty(xdisplay,
                    xwindow,
                    atoms[0],  // name
                    atoms[1],  // type
                    32,  // size in bits of items in 'value'
                    PropModeReplace,
                    nullptr,
                    0);  // num items
  }
}
