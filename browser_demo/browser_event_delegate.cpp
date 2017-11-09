/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "browser_event_delegate.h"

#include <QDebug>
#include <QKeyEvent>
#include <qcef_web_page.h>
#include <qcef_web_view.h>

namespace {

const int kF11Code = 95;
const int kF5Code = 71;

enum MenuIds {
  // Normal navigation.
  MenuBack = QCefContextMenu::MENU_ID_USER_FIRST,
  MenuForward,
  MenuReload,
  MenuStop,

  // Editable.
  MenuUndo,
  MenuRedo,
  MenuCut,
  MenuCopy,
  MenuPaste,
  MenuDelete,
  MenuSelectAll,

  // Link.
  MenuOpenLinkInNewTab,
  MenuCopyLinkAddress,
};

}  // namespace

struct BrowserEventDelegatePrivate {
  QMenu* context_menu = nullptr;
};

BrowserEventDelegate::BrowserEventDelegate(QObject* parent)
    : QObject(parent),
      p_(new BrowserEventDelegatePrivate()) {
  p_->context_menu = new QMenu();
}

BrowserEventDelegate::~BrowserEventDelegate() {
  if (p_ != nullptr) {
    delete p_->context_menu;

    delete p_;
    p_ = nullptr;
  }
}

bool BrowserEventDelegate::onBeforePopup(
    const QUrl& url,
    QCefWindowOpenDisposition disposition) {
  emit this->popupRequested(url, disposition);
  return true;
}

void BrowserEventDelegate::onPreKeyEvent(const QKeyEvent& event) {
  switch (event.nativeScanCode()) {
    case kF5Code: {
      emit this->refreshRequested();
      break;
    }
    case kF11Code: {
      emit this->toggleFullscreen();
      break;
    }
    default: {
      break;
    }
  }
}

bool BrowserEventDelegate::onBeforeBrowse(const QUrl& url, bool is_redirect) {
  if (url.host() == "bing.com") {
    qDebug() << "Prevent browser from navigating to bing.com, " << is_redirect;
    return true;
  }
  return false;
}

void BrowserEventDelegate::onBeforeContextMenu(
    QCefWebPage* web_page,
    QCefContextMenu* menu,
    const QCefContextMenuParams& params) {

  if (params.isEditable()) {
    // Editable menu.
    auto state = params.getEditStateFlags();
    menu->addItem(MenuIds::MenuUndo, "Undo",
                  state & QCEF_CM_EDITFLAG_CAN_UNDO,
                  [](QCefWebPage* page) {
                    page->undo();
                  });
    menu->addItem(MenuIds::MenuRedo, "Redo",
                  state & QCEF_CM_EDITFLAG_CAN_REDO,
                  [](QCefWebPage* page) {
                    page->redo();
                  });
    menu->addSeparator();
    menu->addItem(MenuIds::MenuCut, "Cut",
                  state & QCEF_CM_EDITFLAG_CAN_CUT,
                  [](QCefWebPage* page) {
                    page->cut();
                  });
    menu->addItem(MenuIds::MenuCopy, "Copy",
                  state & QCEF_CM_EDITFLAG_CAN_COPY,
                  [](QCefWebPage* page) {
                    page->copy();
                  });
    menu->addItem(MenuIds::MenuPaste, "Paste",
                  state & QCEF_CM_EDITFLAG_CAN_PASTE,
                  [](QCefWebPage* page) {
                    page->paste();
                  });
    menu->addItem(MenuIds::MenuDelete, "Delete",
                  state & QCEF_CM_EDITFLAG_CAN_DELETE,
                  [](QCefWebPage* page) {
                    page->doDelete();
                  });
    menu->addSeparator();
    menu->addItem(MenuIds::MenuSelectAll, "Select all",
                  state & QCEF_CM_EDITFLAG_CAN_SELECT_ALL,
                  [](QCefWebPage* page) {
                    page->selectAll();
                  });
    return;
  }

  const QCefContextMenuFlags flags = params.getTypeFlags();
  if (flags & QCEF_CM_FLAG_LINK) {
    // Link is selected.
    const QUrl url(params.getLinkUrl());
    menu->addItem(MenuIds::MenuOpenLinkInNewTab, "Open link in new tab", true,
                  [this, url](QCefWebPage* page) {
                    emit this->popupRequested(
                            url,
                            QCEF_WOD_NEW_FOREGROUND_TAB);

                  });
    menu->addItem(MenuIds::MenuCopyLinkAddress, "Copy link address", true,
                  [this, url](QCefWebPage* page) {
                    emit this->copyLinkToClipboard(url);
                  });
    return;
  }

  // Normal menu.
  menu->addItem(MenuIds::MenuBack, "Back", web_page->canGoBack(),
                [](QCefWebPage* page) {
                  page->back();
                });
  menu->addItem(MenuIds::MenuForward, "Forward", web_page->canGoForward(),
                [](QCefWebPage* page) {
                  page->forward();
                });
  if (web_page->isLoading()) {
    menu->addItem(MenuIds::MenuStop, "Stop", true,
                  [](QCefWebPage* page) {
                    page->stop();
                  });
  } else {
    menu->addItem(MenuIds::MenuReload, "Reload", true,
                  [](QCefWebPage* page) {
                    page->reload();
                  });
  }
}
