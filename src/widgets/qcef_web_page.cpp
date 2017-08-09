// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_web_page.h"

#include <QDebug>
#include <QJsonObject>
#include <QSize>
#include <QWebChannel>
#include <QWindow>

#include "include/cef_app.h"
#include "core/qcef_client_handler.h"
#include "core/qcef_util.h"
#include "core/qcef_browser_transport.h"
#include "widgets/qcef_client_handler_delegate.h"
#include "widgets/qcef_web_settings.h"

namespace {

const char kBlankUrl[] = "about:blank";

void MergeWebPageSettings(CefBrowserSettings& cef_settings,
                          const QCefWebSettings& settings) {
  CefString(&cef_settings.default_encoding) = settings.defaultEncoding();
  cef_settings.remote_fonts = static_cast<cef_state_t>(settings.remoteFonts());
  cef_settings.javascript = static_cast<cef_state_t>(settings.javascript());
  cef_settings.javascript_open_windows =
      static_cast<cef_state_t>(settings.javascriptOpenWindows());
  cef_settings.javascript_close_windows =
      static_cast<cef_state_t>(settings.javascriptCloseWindow());
  cef_settings.javascript_access_clipboard =
      static_cast<cef_state_t>(settings.javascriptAccessClipboard());
  cef_settings.javascript_dom_paste =
      static_cast<cef_state_t>(settings.javascriptDomPaster());
  cef_settings.plugins = static_cast<cef_state_t>(settings.plugin());
  cef_settings.universal_access_from_file_urls =
      static_cast<cef_state_t>(settings.universalAccessFromFileUrls());
  cef_settings.file_access_from_file_urls =
      static_cast<cef_state_t>(settings.fileAccessFromFileUrls());
  cef_settings.web_security = static_cast<cef_state_t>(settings.webSecurity());
  cef_settings.image_loading =
      static_cast<cef_state_t>(settings.imageLoading());
  cef_settings.text_area_resize =
      static_cast<cef_state_t>(settings.textAreaResize());
  cef_settings.local_storage =
      static_cast<cef_state_t>(settings.localStorage());
  cef_settings.databases = static_cast<cef_state_t>(settings.databases());
  cef_settings.application_cache =
      static_cast<cef_state_t>(settings.applicationCache());
  cef_settings.webgl = static_cast<cef_state_t>(settings.webGL());
  CefString(&cef_settings.accept_language_list) = settings.acceptLanguageList();
}

}  // namespace

struct QCefWebPagePrivate {
  QUrl url = QUrl(kBlankUrl);
  QString page_error_content;
  QCefClientHandlerDelegate* delegate = nullptr;
  CefRefPtr<QCefClientHandler> client_handler = nullptr;
  QWindow* browser_window_wrapper = nullptr;
  QCefWebSettings* settings = nullptr;
  QWebChannel* channel = nullptr;
  QCefBrowserTransport* transport = nullptr;
};

QCefWebPage::QCefWebPage(QObject* parent)
    : QObject(parent), p_(new QCefWebPagePrivate()) {
  p_->delegate = new QCefClientHandlerDelegate(this);
  p_->client_handler = new QCefClientHandler(p_->delegate);
  p_->settings = new QCefWebSettings();
  p_->channel = new QWebChannel();
}

QCefWebPage::~QCefWebPage() {
  if (p_->delegate != nullptr) {
    delete p_->delegate;
    p_->delegate = nullptr;
  }

  if (p_->client_handler != nullptr) {
    p_->client_handler->Release();
    p_->client_handler = nullptr;
  }

  if (p_->browser_window_wrapper != nullptr) {
    delete p_->browser_window_wrapper;
    p_->browser_window_wrapper = nullptr;
  }

  if (p_->settings != nullptr) {
    delete p_->settings;
    p_->settings = nullptr;
  }

  if (p_->channel != nullptr) {
    delete p_->channel;
    p_->channel = nullptr;
  }

  if (p_->transport != nullptr) {
    delete p_->transport;
    p_->transport = nullptr;
  }

  delete p_;
  p_ = nullptr;
}

void QCefWebPage::load(const QUrl& url) {
  this->setUrl(url);
}

void QCefWebPage::setUrl(const QUrl& url) {
  Q_ASSERT(p_->client_handler != nullptr);
  p_->url = url;
  const std::string url_str = url.toString().toStdString();
  p_->delegate->cef_browser()->GetMainFrame()->LoadURL(url_str);
}

QUrl QCefWebPage::url() const {
  return QUrl();
}

void QCefWebPage::setPageErrorContent(const QString& page_error_content) {
  p_->page_error_content = page_error_content;
}

const QString& QCefWebPage::pageErrorContent() const {
  return p_->page_error_content;
}

QCefWebSettings* QCefWebPage::settings() const {
  return p_->settings;
}

QWebChannel* QCefWebPage::webChannel() const {
  return p_->channel;
}

bool QCefWebPage::canGoBack() const {
  return p_->delegate->cef_browser()->CanGoBack();
}

bool QCefWebPage::canGoForward() const {
  return p_->delegate->cef_browser()->CanGoForward();
}

void QCefWebPage::goBack() {
  p_->delegate->cef_browser()->GoBack();
}

void QCefWebPage::goForward() {
  p_->delegate->cef_browser()->GoForward();
}

void QCefWebPage::reload() {
  p_->delegate->cef_browser()->Reload();
}

void QCefWebPage::reloadIgnoreCache() {
  p_->delegate->cef_browser()->ReloadIgnoreCache();
}

bool QCefWebPage::isLoading() const {
  return p_->delegate->cef_browser()->IsLoading();
}

void QCefWebPage::stopLoad() {
  p_->delegate->cef_browser()->StopLoad();
}

void QCefWebPage::createBrowser(QWindow* parent_window, const QSize& size) {
  CefRect rect;
  rect.x = 0;
  rect.y = 0;
  rect.width = size.width();
  rect.height = size.height();

  CefWindowInfo window_info;
  CefBrowserSettings cef_settings;
  MergeWebPageSettings(cef_settings, *p_->settings);

  const unsigned long wid = InitCefBrowserWindow(size.width(), size.height());
  p_->browser_window_wrapper = QWindow::fromWinId(wid);
  window_info.SetAsChild(wid, rect);

  const std::string url = p_->url.url().toStdString();
  CefBrowserHost::CreateBrowserSync(window_info,
                                    p_->client_handler.get(),
                                    CefString(url),
                                    cef_settings,
                                    nullptr);

  QWindow::fromWinId(wid)->setParent(parent_window);
}


void QCefWebPage::resizeCefBrowser(const QSize& size) {
  SetXWindowBounds(p_->browser_window_wrapper->winId(),
                   0, 0, size.width(), size.height());
  SetXWindowBounds(p_->delegate->cef_browser()->GetHost()->GetWindowHandle(),
                   0, 0, size.width(), size.height());
}

void QCefWebPage::createTransportChannel() {
  Q_ASSERT(p_->transport == nullptr);
  if (p_->transport != nullptr) {
    delete p_->transport;
    p_->transport = nullptr;
  }
  p_->transport = new QCefBrowserTransport(p_->delegate->cef_browser());
  p_->channel->connectTo(p_->transport);
}

void QCefWebPage::releaseTransportChannel() {
  Q_ASSERT(p_->transport != nullptr);
  if (p_->transport != nullptr) {
    p_->channel->disconnectFrom(p_->transport);
    delete p_->transport;
    p_->transport = nullptr;
  }
}

void QCefWebPage::handleWebMessage(const QJsonObject& message) {
#ifdef N_DEBUG
  qDebug() << "WebPage::handleWebMessage():" << message;
#endif
  emit p_->transport->messageReceived(message, p_->transport);
}
