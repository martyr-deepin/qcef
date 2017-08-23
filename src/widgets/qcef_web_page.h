// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_WEB_PAGE_H
#define QCEF_WIDGETS_QCEF_WEB_PAGE_H

#include <QObject>

#include <QUrl>

#include "qcef_widgets_export.h"

class QWindow;
class QWebChannel;
struct QCefWebPagePrivate;
class QCefWebSettings;

// A QCefWebPage object holds web document, history, actions, and provides
// access to cef browser internal states.
class QCEF_WIDGETS_EXPORT QCefWebPage : public QObject {
  Q_OBJECT
  Q_PROPERTY(QUrl url READ url WRITE setUrl)
  Q_PROPERTY(QString page_error_content
             READ pageErrorContent
             WRITE setPageErrorContent)

 public:
  explicit QCefWebPage(QCefWebSettings* settings, QObject* parent = nullptr);
  ~QCefWebPage() override;

  void load(const QUrl& url);
  void setUrl(const QUrl& url);
  QUrl url() const;

  void setPageErrorContent(const QString& page_error_content);
  const QString& pageErrorContent() const;

  // Returns a pointer to the page's settings object.
  QCefWebSettings* settings() const;

  // Returns a pointer to the web channel instance used by this page.
  // Might be null pointer if it has not been initialized.
  QWebChannel* webChannel() const;

  // Runs the JavaScript code contained in |script_source|.
  void runJavaScript(const QString& script_source);
  // Runs the JavaScript code contained in |script_source|.
  // |script_url| is reference of |script_source| which can be identified
  // in web development console.
  void runJavaScript(const QString& script_source, const QString& script_url);

  bool canGoBack() const;
  bool canGoForward() const;
  void goBack();
  void goForward();

  void reload();
  void reloadIgnoreCache();
  bool isLoading() const;
  void stopLoad();

 signals:
  void renderContextCreated();

  void loadStarted();
  void loadingStateChanged(bool is_loading,
                           bool can_go_back,
                           bool can_go_forward);
  void loadFinished(bool ok);

  void fullscreenRequested(bool fullscreen);

  void titleChanged(const QString& title);
  void urlChanged(const QUrl& url);
  void iconChanged(const QIcon& icon);
  // TODO(LiuLang): Remove this signal.
  void iconUrlChanged(const QString& icon_url);

 private:
  friend class QCefWebView;
  friend class QCefClientHandlerDelegate;

  // Create a new CEF browser instance, with initial |size|.
  // This method can only be call once for one QCefWebView object.
  void createBrowser(QWindow* parent_window, const QSize& size);

  void resizeCefBrowser(const QSize& size);

  // Handle messages received from renderer process.
  void createTransportChannel();
  void releaseTransportChannel();
  void handleWebMessage(const QJsonObject& message);

  QCefWebPagePrivate* p_ = nullptr;
};


#endif  // QCEF_WIDGETS_QCEF_WEB_PAGE_H
