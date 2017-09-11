// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
  void resizeEvent(QResizeEvent* event) override;

 public:
  bool eventFilter(QObject* watched, QEvent* event) override;

 private:
  QCefWebViewPrivate* p_ = nullptr;
};


#endif  // QCEF_WIDGETS_QCEF_WEB_VIEW_H
