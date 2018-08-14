/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#include "qcef_native_event_filter.h"

#include <QApplication>
#include <QX11Info>
#include <QWidget>
#include <xcb/xcb.h>
#include <X11/Xlib.h>

namespace {

QCefNativeEventFilter* g_filter = nullptr;

// Get currently focused window, might be qt window or cef window.
::Window GetFocusWindow() {
  ::Display* display = QX11Info::display();
  ::Window window;
  int revert_to;
  XGetInputFocus(display, &window, &revert_to);
  return window;
}

// Get top level qt window.
::Window GetTopLevelWindow() {
  if (qApp->focusWidget() != nullptr &&
      qApp->focusWidget()->topLevelWidget() != nullptr) {
    return qApp->focusWidget()->topLevelWidget()->winId();
  } else {
    return 0;
  }
}

void FocusToWindow(::Window window) {
  ::Display* display = QX11Info::display();
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
}

}  // namespace

// static
void QCefNativeEventFilter::install() {
  if (g_filter == nullptr) {
    g_filter = new QCefNativeEventFilter();
    Q_ASSERT(g_filter != nullptr);
    // 禁用，使用Qt本地事件实现鼠标按下后转移窗口焦点
//    qApp->installNativeEventFilter(g_filter);
  }
}

bool QCefNativeEventFilter::nativeEventFilter(const QByteArray& event_type,
                                          void* message,
                                          long* result) {
  Q_UNUSED(result);
  if ("xcb_generic_event_t" == event_type) {
    xcb_generic_event_t* event = static_cast<xcb_generic_event_t*>(message);
    if ((event->response_type & ~0x80) == XCB_GE_GENERIC) {
      xcb_ge_generic_event_t* ge_event =
          reinterpret_cast<xcb_ge_generic_event_t*>(event);
      const ::Window top_level_win = GetTopLevelWindow();
      if ((ge_event->event_type == XCB_BUTTON_PRESS) &&
          (top_level_win != 0) &&
          (top_level_win != GetFocusWindow())) {
        // Focus on qt top level window.
        FocusToWindow(top_level_win);
      }
    }
  }
  return false;
}
