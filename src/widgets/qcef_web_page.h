// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_WEB_PAGE_H
#define QCEF_WIDGETS_QCEF_WEB_PAGE_H

#include <QObject>
#include <QIcon>
#include <QUrl>

#include "qcef_ssl_status.h"
#include "qcef_widgets_export.h"

class QWindow;
class QWebChannel;
struct QCefWebPagePrivate;
class QCefWebSettings;
class QCefBrowserEventDelegate;

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

  // This property holds the zoom factor for the page content.
  Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor)

 public:
  explicit QCefWebPage(QObject* parent = nullptr);
  ~QCefWebPage() override;

  void load(const QUrl& url);
  void setUrl(const QUrl& url);

  // Load the contents of |html| with the specified dummy |url|.
  // |url| should have a standard scheme (for example, http scheme)
  // or behaviors like link clicks and web security restrictions
  // may not behave as expected.
  void setHtml(const QString& html, const QUrl& url = QUrl());

  QIcon icon() const;
  QUrl iconUrl() const;
  QString title() const;
  QUrl url() const;
  qreal zoomFactor() const;

  void setPageErrorContent(const QString& page_error_content);
  const QString& pageErrorContent() const;

  // Returns a pointer to the page's settings object.
  // Update browser settings before loading any url or html content.
  QCefWebSettings* settings() const;

  // Returns a pointer to the web channel instance used by this page.
  // Might be null pointer if it has not been initialized.
  QWebChannel* webChannel() const;

  // Returns the view widget that is associated with the web page.
  QWidget* view() const;

  // Runs the JavaScript code contained in |script_source|.
  void runJavaScript(const QString& script_source);
  // Runs the JavaScript code contained in |script_source|.
  // |script_url| is reference of |script_source| which can be identified
  // in web development console.
  void runJavaScript(const QString& script_source, const QString& script_url);

  bool canGoBack() const;
  bool canGoForward() const;
  bool isLoading() const;

  // Asynchronous method to retrieve the page's content as HTML,
  // enclosed in HTML and BODY tags. Upon successful completion,
  // resultCallback is called with the page's content.
  void toHtml(void (* callback)(const QString& html)) const;

  // Asynchronous method to retrieve the page's content converted
  // to plain text, completely stripped of all HTML formatting.
  // Upon successful completion, resultCallback is called
  // with the page's content.
  void toPlainText(void (* callback)(const QString& text)) const;

  // Change the zoom level to the specified value. Specify 0.0 to reset the
  // zoom level. If called on the UI thread the change will be applied
  // immediately. Otherwise, the change will be applied asynchronously on the
  // UI thread.
  void setZoomFactor(qreal factor);
  void resetZoomFactor();
  void zoomIn();
  void zoomOut();

  // Editing.
  void undo();
  void redo();
  void cut();
  void copy();
  void paste();
  void doDelete();
  void selectAll();

  // Returns ssl status of current page.
  QCefSSLStatus getSSLStatus() const;

  // Get current event delegate, default is nullptr.
  // Note that QCefWebPage does not take ownership of event delegate.
  QCefBrowserEventDelegate* getEventDelegate() const;
  void setEventDelegate(QCefBrowserEventDelegate* delegate);

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

 public slots:
  void back();
  void forward();
  void reload();
  void reloadIgnoreCache();
  void stop();

 private:
  friend class QCefWebView;
  friend class QCefClientHandlerDelegate;

  void onBrowserGotFocus();

  void resizeBrowserWindow(const QSize& size);
  void updateBrowserGeometry();

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
