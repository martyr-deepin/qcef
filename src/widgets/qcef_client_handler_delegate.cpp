// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_client_handler_delegate.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/qcef_web_channel_consts.h"
#include "include/cef_origin_whitelist.h"
#include "widgets/qcef_web_settings.h"

QCefClientHandlerDelegate::QCefClientHandlerDelegate(QCefWebPage* web_page)
  : web_page_(web_page){
}

QCefClientHandlerDelegate::~QCefClientHandlerDelegate() {
  if (cef_browser_ != nullptr) {
    cef_browser_->GetHost()->CloseBrowser(true);
    cef_browser_->Release();
    cef_browser_ = nullptr;
  }
}

void QCefClientHandlerDelegate::OnBeforePopup(const CefString& target_url) {
  // TODO(LiuLang): Add option.
  if (cef_browser_ != nullptr) {
    cef_browser_->GetMainFrame()->LoadURL(target_url);
  }
}

void
QCefClientHandlerDelegate::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
  if (cef_browser_ == nullptr) {
    cef_browser_ = browser;
  }

  // Set Cross-Origin white list.
  const auto white_list = web_page_->settings()->crossOriginWhiteList();
  for (const QCefWebSettings::CrossOriginEntry& entry : white_list) {
    qDebug() << "Add cross-origin white entry:" << entry.source << entry.target;
    CefAddCrossOriginWhitelistEntry(entry.source.toString().toStdString(),
                                    entry.target.scheme().toStdString(),
                                    entry.target.host().toStdString(),
                                    true);
  }
}

void QCefClientHandlerDelegate::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  qDebug() << "QCefClientHandlerDelegate::OnBeforeClose" << browser
           << cef_browser_;
  if (cef_browser_->GetIdentifier() == browser->GetIdentifier()) {
    cef_browser_ = nullptr;
    // TODO(LiuLang): Emit close signal.
  }
}

void QCefClientHandlerDelegate::OnFaviconURLChange(const CefString& icon_url,
                                                   CefRefPtr<CefImage> icon) {
  QPixmap pixmap;
  int pixel_width = 0;
  int pixel_height = 0;
  const int scale_factor = 1;
  CefRefPtr<CefBinaryValue> binary = icon->GetAsPNG(scale_factor, true,
                                                    pixel_width, pixel_height);
  if (binary != nullptr) {
    const size_t image_size = binary->GetSize();
    unsigned char* data = (unsigned char *) malloc(image_size);
    const size_t read = binary.get()->GetData(data, image_size, 0);
    pixmap.loadFromData(data, static_cast<uint>(read));
    free(data);
  }
  const QUrl url(QString::fromStdString(icon_url));
  if (!pixmap.isNull()) {
    QIcon qicon(pixmap);
    web_page_->updateFavicon(url, qicon);
  } else {
    web_page_->updateFavicon(url, QIcon());
  }
}

void QCefClientHandlerDelegate::OnGotFocus(CefRefPtr<CefBrowser> browser) {
  if (cef_browser_ != nullptr &&
      browser->GetIdentifier() == cef_browser_->GetIdentifier()) {
    web_page_->onBrowserGotFocus();
  }
}

void QCefClientHandlerDelegate::OnLoadStarted(CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame) {
  if (cef_browser_ != nullptr &&
      browser->GetIdentifier() == cef_browser_->GetIdentifier() &&
      browser->GetMainFrame()->GetIdentifier() == frame->GetIdentifier()) {
    emit web_page_->loadStarted();
  }

  web_page_->updateBrowserWindowGeometry();
}

void QCefClientHandlerDelegate::OnLoadingStateChange(
    CefRefPtr<CefBrowser> browser,
    bool is_loading,
    bool can_go_back,
    bool can_go_forward) {
  if (cef_browser_ != nullptr &&
      browser->GetIdentifier() == cef_browser_->GetIdentifier()) {
    emit web_page_->loadingStateChanged(is_loading,
                                        can_go_back,
                                        can_go_forward);
  }
}

void QCefClientHandlerDelegate::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          int httpStatusCode) {
  Q_UNUSED(httpStatusCode);
  if (cef_browser_ != nullptr &&
      browser->GetIdentifier() == cef_browser_->GetIdentifier() &&
      browser->GetMainFrame()->GetIdentifier() == frame->GetIdentifier()) {
    emit web_page_->loadFinished(true);
  }
}

std::string QCefClientHandlerDelegate::OnLoadError(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int errorCode) {
  Q_UNUSED(errorCode);
  if (cef_browser_ != nullptr &&
      browser->GetIdentifier() == cef_browser_->GetIdentifier() &&
      browser->GetMainFrame()->GetIdentifier() == frame->GetIdentifier()) {
    emit web_page_->loadFinished(false);
  }

  // TODO(LiuLang): Pass |errorCode|.
  return web_page_->pageErrorContent().toStdString();
}

bool QCefClientHandlerDelegate::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) {
  Q_UNUSED(source_process);

  if (cef_browser_ != nullptr &&
      browser->GetIdentifier() != cef_browser_->GetIdentifier()) {
    return false;
  }

  const std::string name(message->GetName());
  if (name == kQCefRenderContextCreated) {
    web_page_->createTransportChannel();
    emit web_page_->renderContextCreated();
    return true;
  }
  if (name == kQCefRenderContextReleased) {
    web_page_->releaseTransportChannel();
    return true;
  }
  if (name == kQCefRenderQtMessage) {
    CefRefPtr<CefListValue> args = message->GetArgumentList();
    if (args->GetSize() != 1) {
      qWarning() << __FUNCTION__ << "args size mismatch, expect 1, got"
                 << args->GetSize();
      return false;
    }
    const QString msg = QString::fromStdString(args->GetString(0));
    qDebug() << __FUNCTION__ << " message :" << msg;
    const QJsonDocument doc(QJsonDocument::fromJson(msg.toUtf8()));
    if (doc.isObject()) {
      web_page_->handleWebMessage(doc.object());
    } else {
      qWarning() << __FUNCTION__ << " invalid json message:" << msg;
    }
    return true;
  }

  return false;
}

void QCefClientHandlerDelegate::OnSetFullscreen(bool fullscreen) {
  emit web_page_->fullscreenRequested(fullscreen);
}

void QCefClientHandlerDelegate::OnTitleChanged(const CefString& title) {
  web_page_->updateTitle(QString::fromStdString(title));
}

void QCefClientHandlerDelegate::OnUrlChanged(const CefString& url) {
  web_page_->updateUrl(QUrl(QString::fromStdString(url)));
}