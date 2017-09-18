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

#ifndef QCEF_BROWSER_EVENT_DELEGATE_H
#define QCEF_BROWSER_EVENT_DELEGATE_H

#include <QObject>
#include <qcef_browser_event_delegate.h>

struct BrowserEventDelegatePrivate;

class BrowserEventDelegate : public QObject,
                             public QCefBrowserEventDelegate {
 Q_OBJECT

 public:
  explicit BrowserEventDelegate(QObject* parent = nullptr);

  ~BrowserEventDelegate() override;

  bool onBeforeBrowse(const QUrl& url, bool is_redirect) override;

  void onBeforeContextMenu(QCefWebPage* web_page,
                           QCefContextMenu* menu,
                           const QCefContextMenuParams& params) override;

  bool onBeforePopup(const QUrl& url,
                     QCefWindowOpenDisposition disposition) override;

  void onPreKeyEvent(const QKeyEvent& event) override;

 signals:
  void popupRequested(const QUrl& url, QCefWindowOpenDisposition disposition);
  void copyLinkToClipboard(const QUrl& url);

  void refreshRequested();
  void toggleFullscreen();

 private:
  BrowserEventDelegatePrivate* p_ = nullptr;
};

#endif  // QCEF_BROWSER_EVENT_DELEGATE_H
