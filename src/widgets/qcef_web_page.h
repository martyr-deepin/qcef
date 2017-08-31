// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_WEB_PAGE_H
#define QCEF_WIDGETS_QCEF_WEB_PAGE_H

#include <QObject>
#include <QIcon>
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

  // This property holds the title of the web page currently viewed
  // By default, this property contains an empty string.
  Q_PROPERTY(QString title READ title NOTIFY titleChanged)

  // This property holds the URL of the web page currently viewed
  // Setting this property clears the view and loads the URL.*
  // By default, this property contains an empty, invalid URL.
  Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

  // This property holds the icon associated with the page currently viewed
  // By default, this property contains a null icon.
  Q_PROPERTY(QIcon icon READ icon NOTIFY iconChanged)

  // This property holds the URL of the icon associated with the page currently
  // viewed. By default, this property contains an empty URL.
  Q_PROPERTY(QUrl iconUrl READ iconUrl NOTIFY iconUrlChanged)

  Q_PROPERTY(QString page_error_content
             READ pageErrorContent
             WRITE setPageErrorContent)

 public:
  explicit QCefWebPage(QObject* parent = nullptr);
  ~QCefWebPage() override;

  void load(const QUrl& url);
  void setUrl(const QUrl& url);

  QIcon icon() const;
  QUrl iconUrl() const;
  QString title() const;
  QUrl url() const;

  void setPageErrorContent(const QString& page_error_content);
  const QString& pageErrorContent() const;

  // Returns a pointer to the page's settings object.
  // Update browser settings before loading any url or html content.
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
  void back();
  void forward();

  void reload();
  void reloadIgnoreCache();
  bool isLoading() const;
  void stop();

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

  // Notified when page icon is updated.
  void iconChanged(const QIcon& icon);

  // Notified when page icon is changed.
  void iconUrlChanged(const QUrl& icon_url);

 private:
  friend class QCefWebView;
  friend class QCefClientHandlerDelegate;

  // Create a new CEF browser instance, with initial |size|.
  // This method can only be call once for one QCefWebView object.
  void createBrowser(QWindow* parent_window, const QSize& size);

  void onBrowserGotFocus();

  // Resize browser window on page loading.
  // NOTE(Deepin Ltd.): This hacking tip solves tooltip and IME position issue.
  // A better solution shall be provided, by updating XWindow property.
  void updateBrowserWindowGeometry();

  void resizeCefBrowser(const QSize& size);

  // Handle messages received from renderer process.
  void createTransportChannel();
  void releaseTransportChannel();
  void handleWebMessage(const QJsonObject& message);

  void updateFavicon(const QUrl& url, const QIcon& icon);
  void updateTitle(const QString& title);
  void updateUrl(const QUrl& url);

  QCefWebPagePrivate* p_ = nullptr;
};


#endif  // QCEF_WIDGETS_QCEF_WEB_PAGE_H
