// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_WEB_VIEW_H
#define QCEF_WIDGETS_QCEF_WEB_VIEW_H

#include <QUrl>
#include <QWidget>

#include "base/qcef_export.h"

struct QCefWebViewPrivate;
class QCefWebPage;

// This widget class is used to view web document.
class QCEF_EXPORT QCefWebView : public QWidget {
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
  // Resize cef window.
  void resizeEvent(QResizeEvent* event) override;

 private:
  QCefWebViewPrivate* p_ = nullptr;
};


#endif  // QCEF_WIDGETS_QCEF_WEB_VIEW_H
