// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H
#define QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H

#include <QKeyEvent>
#include <QUrl>
#include "qcef_widgets_export.h"

// The manner in which a link click should be opened. These constants match
// their equivalents in Chromium's window_open_disposition.h and should not be
// renumbered.
enum class QCEF_WIDGETS_EXPORT QCefWindowOpenDisposition {
  UNKNOWN,
  CURRENT_TAB,
  SINGLETON_TAB,
  NEW_FOREGROUND_TAB,
  NEW_BACKGROUND_TAB,
  NEW_POPUP,
  NEW_WINDOW,
  SAVE_TO_DISK,
  OFF_THE_RECORD,
  IGNORE_ACTION
};

// Implement this class to handle mouse/keyboard events in cef browser window.
class QCEF_WIDGETS_EXPORT QCefBrowserEventDelegate {
 public:
  // Return true to cancel the navigation or false to allow the navigation
  // to proceed. |url| is the target url to be navigated to.
  virtual bool onBeforeBrowse(const QUrl& url, bool is_redirect) = 0;

  // This method is called before a new popup browser is created.
  // |url| is the position new popup window should navigate.
  // Returns true to popup default popup window.
  virtual bool onBeforePopup(const QUrl& url,
                             QCefWindowOpenDisposition disposition) = 0;

  virtual bool onPreKeyEvent(const QKeyEvent& event) = 0;
};


#endif  // QCEF_WIDGETS_QCEF_BROWSER_EVENT_DELEGATE_H
