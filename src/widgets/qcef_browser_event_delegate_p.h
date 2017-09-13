// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_P_H
#define QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_P_H

#include "include/cef_context_menu_handler.h"

struct QCefContextMenuParamsPrivate {
  CefRefPtr<CefContextMenuParams> params = nullptr;
};

#endif  // QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_P_H