// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "browser_tab_bar.h"

#include <QDebug>

BrowserTabBar::BrowserTabBar(QWidget* parent) : QTabBar(parent) {

}

void BrowserTabBar::contextMenuEvent(QContextMenuEvent* event) {
  QWidget::contextMenuEvent(event);
  qDebug() << "context menu";
  // TODO(LiuLang): show context menu.
}
