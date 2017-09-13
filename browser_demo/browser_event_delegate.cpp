// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "browser_event_delegate.h"

#include <QDebug>
#include <QKeyEvent>
#include <qcef_web_page.h>
#include <qcef_web_view.h>

namespace {

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

bool BrowserEventDelegate::onPreKeyEvent(const QKeyEvent& event) {
  Q_UNUSED(event);
  return false;
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
                  state & QCefContextMenuEditFlags::QCM_EDITFLAG_CAN_UNDO,
                  [](QCefWebPage* page) {
                    page->undo();
                  });
    menu->addItem(MenuIds::MenuRedo, "Redo",
                  state & QCefContextMenuEditFlags::QCM_EDITFLAG_CAN_REDO,
                  [](QCefWebPage* page) {
                    page->redo();
                  });
    menu->addSeparator();
    menu->addItem(MenuIds::MenuCut, "Cut",
                  state & QCefContextMenuEditFlags::QCM_EDITFLAG_CAN_CUT,
                  [](QCefWebPage* page) {
                    page->cut();
                  });
    menu->addItem(MenuIds::MenuCopy, "Copy",
                  state & QCefContextMenuEditFlags::QCM_EDITFLAG_CAN_COPY,
                  [](QCefWebPage* page) {
                    page->copy();
                  });
    menu->addItem(MenuIds::MenuPaste, "Paste",
                  state & QCefContextMenuEditFlags::QCM_EDITFLAG_CAN_PASTE,
                  [](QCefWebPage* page) {
                    page->paste();
                  });
    menu->addItem(MenuIds::MenuDelete, "Delete",
                  state & QCefContextMenuEditFlags::QCM_EDITFLAG_CAN_DELETE,
                  [](QCefWebPage* page) {
                    page->doDelete();
                  });
    menu->addSeparator();
    menu->addItem(MenuIds::MenuSelectAll, "Select all",
                  state & QCefContextMenuEditFlags::QCM_EDITFLAG_CAN_SELECT_ALL,
                  [](QCefWebPage* page) {
                    page->selectAll();
                  });
    return;
  }

  const QCefContextMenuFlags flags = params.getTypeFlags();
  if (flags & QCefContextMenuFlags::QCM_FLAG_LINK) {
    // Link is selected.
    const QUrl url(params.getLinkUrl());
    menu->addItem(MenuIds::MenuOpenLinkInNewTab, "Open link in new tab", true,
                  [this, url](QCefWebPage* page) {
                    emit this->popupRequested(
                            url,
                            QCefWindowOpenDisposition::NEW_FOREGROUND_TAB);

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
