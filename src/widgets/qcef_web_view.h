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

#ifndef QCEF_WIDGETS_QCEF_WEB_VIEW_H
#define QCEF_WIDGETS_QCEF_WEB_VIEW_H

#include <QWidget>

#include <QUrl>

#include "qcef_widgets_export.h"

struct QCefWebViewPrivate;
class QCefWebPage;
class QCefWebSettings;

// This widget class is used to view web document.
// When inheriting from QCefWebView, remember to disable rtti feature
// by appending -fno-rtti to compiler flag list, or else an error like
// "undefined reference to `typeinfo for QCefWebView'" will occur.
class QCEF_WIDGETS_EXPORT QCefWebView : public QWidget {
  Q_OBJECT
  Q_PROPERTY(QUrl url READ url WRITE setUrl)

 public:
  explicit QCefWebView(QWidget* parent = nullptr);
  ~QCefWebView() override;

  // Loads the specified |url| and displays it.
  void load(const QUrl& url);

  // Get/set url of web page.
  void setUrl(const QUrl& url);
  QUrl url() const;

  QCefWebPage* page() const;

 protected:
  // Refresh geometry of web content to fix browser window focus issue.
  bool event(QEvent* event) override;

 private:
  QCefWebViewPrivate* p_ = nullptr;
};


#endif  // QCEF_WIDGETS_QCEF_WEB_VIEW_H
