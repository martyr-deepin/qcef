/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#include "widgets/qcef_web_page.h"

#include <QApplication>
#include <QDebug>
#include <QJsonObject>
#include <QLayout>
#include <QWebChannel>
#include <QWidget>
#include <QWindow>

#include "core/qcef_browser_transport.h"
#include "core/qcef_client_handler.h"
#include "core/qcef_string_visitor.h"
#include "core/qcef_x11_util.h"
#include "widgets/qcef_client_handler_delegate.h"
#include "widgets/qcef_native_event_filter.h"
#include "widgets/qcef_web_settings.h"

namespace {

const char kBlankUrl[] = "about:blank";

void MergeWebPageSettings(CefBrowserSettings& cef_settings,
                          const QCefWebSettings& settings) {
  CefString(&cef_settings.standard_font_family) =
      settings.standardFontFamily().toStdString();
  CefString(&cef_settings.fixed_font_family) =
      settings.fixedFontFamily().toStdString();
  CefString(&cef_settings.serif_font_family) =
      settings.serifFontFamily().toStdString();
  CefString(&cef_settings.sans_serif_font_family) =
      settings.sansSerifFontFamily().toStdString();
  CefString(&cef_settings.cursive_font_family) =
      settings.cursiveFontFamily().toStdString();
  CefString(&cef_settings.fantasy_font_family) =
      settings.fantasyFontFamily().toStdString();
  cef_settings.default_font_size = settings.defaultFontSize();
  cef_settings.default_fixed_font_size = settings.defaultFixedFontSize();
  cef_settings.minimum_font_size = settings.minimumFontSize();
  cef_settings.minimum_logical_font_size = settings.minimumLogicalFontSize();

  CefString(&cef_settings.default_encoding) =
      settings.defaultEncoding().toStdString();
  cef_settings.remote_fonts = static_cast<cef_state_t>(settings.remoteFonts());
  cef_settings.javascript = static_cast<cef_state_t>(settings.javascript());
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
  CefString(&cef_settings.accept_language_list) =
      settings.acceptLanguageList().toStdString();
}

}  // namespace

struct QCefWebPagePrivate {
  ~QCefWebPagePrivate();

  QWidget* view = nullptr;
  WId browser_wid = 0;
  WId parent_window = 0;
  QUrl url;
  QString html;
  QUrl iconUrl;
  QIcon icon;
  QString title;
  QString page_error_content;
  QCefClientHandlerDelegate* delegate = nullptr;
  CefRefPtr<QCefClientHandler> client_handler = nullptr;
  QCefWebSettings* settings = nullptr;
  QWebChannel* channel = nullptr;
  QCefBrowserTransport* transport = nullptr;
  bool channel_connected = false;
  QCefBrowserEventDelegate* event_delegate = nullptr;

  CefRefPtr<CefBrowser> browser();

 private:
  CefRefPtr<CefBrowser> browser_ = nullptr;

  void createBrowserWidget();
};

QCefWebPagePrivate::~QCefWebPagePrivate() {
  view = nullptr;
  // Cef Browser is released in delegate.
  browser_ = nullptr;
  if (settings != nullptr) {
    delete settings;
    settings = nullptr;
  }

  if (delegate != nullptr) {
    delete delegate;
    delegate = nullptr;
  }

  if (channel != nullptr) {
    if (transport != nullptr) {
      channel->disconnectFrom(transport);
    }
    delete channel;
    channel = nullptr;
  }
  if (transport != nullptr) {
    delete transport;
    transport = nullptr;
  }
  if (client_handler != nullptr) {
    client_handler = nullptr;
  }

  event_delegate = nullptr;
}

CefRefPtr<CefBrowser> QCefWebPagePrivate::browser() {
  if (browser_ == nullptr) {
    this->createBrowserWidget();
  }
  return browser_;
}

void QCefWebPagePrivate::createBrowserWidget() {
  parent_window = InitCefBrowserWindow(100, 100);

  CefWindowInfo window_info;
  const CefRect rect{0, 0, 100, 100};
  window_info.SetAsChild(parent_window, rect);
  CefBrowserSettings cef_settings;
  MergeWebPageSettings(cef_settings, *settings);

  browser_ = CefBrowserHost::CreateBrowserSync(window_info,
                                               client_handler.get(),
                                               CefString(""),
                                               cef_settings,
                                               nullptr);
  browser_wid = browser_->GetHost()->GetWindowHandle();
  // Create transport object when browser is ready.
  transport = new QCefBrowserTransport(browser_);
}

QCefWebPage::QCefWebPage(QObject* parent)
    : QObject(parent),
      p_(new QCefWebPagePrivate()) {
  p_->view = static_cast<QWidget*>(parent);
  p_->delegate = new QCefClientHandlerDelegate(this);
  p_->client_handler = new QCefClientHandler(p_->delegate);
  p_->settings = new QCefWebSettings();
  p_->channel = new QWebChannel();

  qApp->installEventFilter(this);
  QCefNativeEventFilter::install();
}

QCefWebPage::~QCefWebPage() {
  if (p_ != nullptr) {
    delete p_;
    p_ = nullptr;
  }
}

void QCefWebPage::load(const QUrl& url) {
  this->setUrl(url);
}

void QCefWebPage::setUrl(const QUrl& url) {
  p_->url = url;

  // Reset html content.
  p_->html.clear();

  p_->browser()->GetMainFrame()->LoadURL(url.toString().toStdString());
}

void QCefWebPage::setHtml(const QString& html, const QUrl& url) {
  p_->html = html;
  if (url.isEmpty()) {
    qWarning() << Q_FUNC_INFO << "url is empty";
    p_->url = kBlankUrl;
  } else {
    p_->url = url;
  }

  // FIXME(Shaohua): Call LoadURL() before LoadString()
  const std::string l = url.toString().toStdString();
  p_->browser()->GetMainFrame()->LoadURL(l);
  p_->browser()->GetMainFrame()->LoadString(html.toStdString(), l);
}

void QCefWebPage::setZoomFactor(qreal factor) {
  p_->browser()->GetHost()->SetZoomLevel(factor);
}

void QCefWebPage::resetZoomFactor() {
  p_->browser()->GetHost()->SetZoomLevel(0.0);
}

void QCefWebPage::zoomIn() {
  auto host = p_->browser()->GetHost();
  const qreal factor = host->GetZoomLevel() + 0.25;
  p_->browser()->GetHost()->SetZoomLevel(factor);
}

void QCefWebPage::zoomOut() {
  auto host = p_->browser()->GetHost();
  const qreal factor = host->GetZoomLevel() - 0.25;
  p_->browser()->GetHost()->SetZoomLevel(factor);
}

void QCefWebPage::undo() {
  p_->browser()->GetFocusedFrame()->Undo();
}

void QCefWebPage::redo() {
  p_->browser()->GetFocusedFrame()->Redo();
}

void QCefWebPage::cut() {
  p_->browser()->GetFocusedFrame()->Cut();
}

void QCefWebPage::copy() {
  p_->browser()->GetFocusedFrame()->Copy();
}

void QCefWebPage::paste() {
  p_->browser()->GetFocusedFrame()->Paste();
}

void QCefWebPage::doDelete() {
  p_->browser()->GetFocusedFrame()->Delete();
}

void QCefWebPage::selectAll() {
  p_->browser()->GetFocusedFrame()->SelectAll();
}

qreal QCefWebPage::zoomFactor() const {
  return p_->browser()->GetHost()->GetZoomLevel();
}

QIcon QCefWebPage::icon() const {
  return p_->icon;
}

QUrl QCefWebPage::iconUrl() const {
  return p_->iconUrl;
}

QString QCefWebPage::title() const {
  return p_->title;
}

QUrl QCefWebPage::url() const {
  return p_->url;
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

QWidget* QCefWebPage::view() const {
  return p_->view;
}

void QCefWebPage::runJavaScript(const QString& script_source) {
  p_->browser()->GetMainFrame()->ExecuteJavaScript(script_source.toStdString(),
                                                   "", 0);
}

void QCefWebPage::runJavaScript(const QString& script_source,
                                const QString& script_url) {
  p_->browser()->GetMainFrame()->ExecuteJavaScript(script_source.toStdString(),
                                                   script_url.toStdString(),
                                                   0);
}

bool QCefWebPage::canGoBack() const {
  return p_->browser()->CanGoBack();
}

bool QCefWebPage::canGoForward() const {
  return p_->browser()->CanGoForward();
}

void QCefWebPage::back() {
  p_->browser()->GoBack();
}

void QCefWebPage::forward() {
  p_->browser()->GoForward();
}

void QCefWebPage::reload() {
  p_->browser()->Reload();
}

void QCefWebPage::reloadIgnoreCache() {
  p_->browser()->ReloadIgnoreCache();
}

bool QCefWebPage::isLoading() const {
  return p_->browser()->IsLoading();
}

void QCefWebPage::stop() {
  p_->browser()->StopLoad();
}

void QCefWebPage::toHtml(Callback callback) const {
  p_->browser()->GetMainFrame()->GetSource(new StringVisitor(callback));
}

void QCefWebPage::toPlainText(Callback callback) const {
  p_->browser()->GetMainFrame()->GetText(new StringVisitor(callback));
}

QCefSSLStatus QCefWebPage::getSSLStatus() const {
  QCefSSLStatus ssl_status{};
  // Returns a nullptr if navigation entry list is empty.
  auto navigation = p_->browser()->GetHost()->GetVisibleNavigationEntry();
  if (navigation.get() != nullptr) {
    CefRefPtr<CefSSLStatus> cef_ssl_status = navigation->GetSSLStatus();
    ssl_status.is_secure_connection = cef_ssl_status->IsSecureConnection();
    ssl_status.content_status = static_cast<QCefSSLContentStatus>(
        cef_ssl_status->GetContentStatus());
  }
  return ssl_status;
}

QCefBrowserEventDelegate* QCefWebPage::getEventDelegate() const {
  return p_->event_delegate;
}

void QCefWebPage::setEventDelegate(QCefBrowserEventDelegate* delegate) {
  p_->event_delegate = delegate;
}

void QCefWebPage::remapBrowserWindow(WId parent_window) {
  ReparentWindow(parent_window, p_->parent_window);
}

void QCefWebPage::updateBrowserGeometry(const QSize& size) {
  const qreal ratio = view()->devicePixelRatioF();
  const int width = static_cast<int>(ratio * size.width());
  const int height = static_cast<int>(ratio * size.height());
  SetXWindowBounds(p_->browser_wid, 0, 0, width, height);
  SetXWindowBounds(p_->parent_window, 0, 0, width, height);
  p_->browser()->GetHost()->NotifyMoveOrResizeStarted();
}

void QCefWebPage::connectTransportChannel() {
  Q_ASSERT(p_->transport != nullptr);
  Q_ASSERT(p_->channel != nullptr);
  p_->channel->connectTo(p_->transport);
  p_->channel_connected = true;
}

void QCefWebPage::disconnectTransportChannel() {
  Q_ASSERT(p_->transport != nullptr);
  Q_ASSERT(p_->channel != nullptr);
  p_->channel_connected = false;
  p_->channel->disconnectFrom(p_->transport);
}

void QCefWebPage::handleWebMessage(const QJsonObject& message) {
  if (p_->transport != nullptr && p_->channel_connected) {
    emit p_->transport->messageReceived(message, p_->transport);
  } else {
    qCritical() << __FUNCTION__ << "transport is null!";
  }
}

void QCefWebPage::updateFavicon(const QUrl& url, const QIcon& icon) {
  p_->iconUrl = url;
  p_->icon = icon;
  emit this->iconChanged(p_->icon);
  emit this->iconUrlChanged(p_->iconUrl);
}

void QCefWebPage::updateTitle(const QString& title) {
  p_->title = title;
  emit this->titleChanged(p_->title);
}

void QCefWebPage::updateUrl(const QUrl& url) {
  p_->url = url;
  emit this->urlChanged(p_->url);
}

bool QCefWebPage::eventFilter(QObject* watched, QEvent* event) {
  switch (event->type()) {
    case QEvent::Move: {
      this->updateBrowserGeometry(QSize(view()->width(), view()->height() + 1));
      this->updateBrowserGeometry(view()->size());
      break;
    }
    default: {
    }
  }
  return QObject::eventFilter(watched, event);
}
