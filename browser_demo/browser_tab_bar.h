// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_BROWSER_TAB_BAR_H
#define QCEF_BROWSER_TAB_BAR_H

#include <QTabBar>

class BrowserTabBar : public QTabBar {
  Q_OBJECT
 public:
  explicit BrowserTabBar(QWidget* parent = nullptr);

 protected:
  void contextMenuEvent(QContextMenuEvent* event) override;
};


#endif //CEF_BROWSER_TAB_BAR_H
