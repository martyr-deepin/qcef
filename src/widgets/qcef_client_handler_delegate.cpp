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

#include "widgets/qcef_client_handler_delegate.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/qcef_web_channel_consts.h"
#include "include/cef_origin_whitelist.h"
#include "qcef_browser_event_delegate.h"
#include "qcef_browser_event_delegate_p.h"
#include "widgets/qcef_web_settings.h"

QCefClientHandlerDelegate::QCefClientHandlerDelegate(QCefWebPage* web_page)
    : web_page_(web_page) {
}

QCefClientHandlerDelegate::~QCefClientHandlerDelegate() {
  if (cef_browser_ != nullptr) {
    auto host = cef_browser_->GetHost();
    cef_browser_ = nullptr;
    host->CloseBrowser(true);
  }

  if (context_menu_ != nullptr) {
    delete context_menu_;
    context_menu_ = nullptr;
  }
}

bool QCefClientHandlerDelegate::OnBeforePopup(
    const CefString& target_url,
    CefLifeSpanHandler::WindowOpenDisposition target_dispositio) {
  if (cef_browser_ != nullptr) {
    const QUrl url(target_url.ToString().c_str());
    QCefWindowOpenDisposition disposition =
        static_cast<QCefWindowOpenDisposition>(target_dispositio);
    auto event_delegate = web_page_->getEventDelegate();
    if (event_delegate != nullptr) {
      return event_delegate->onBeforePopup(url, disposition);
    }
  }
  return true;
}

void
QCefClientHandlerDelegate::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
  if (cef_browser_ == nullptr) {
    cef_browser_ = browser;
  }

  // Set Cross-Origin white list.
  const auto white_list = web_page_->settings()->crossOriginWhiteList();
  for (const QCefWebSettings::CrossOriginEntry& entry : white_list) {
    CefAddCrossOriginWhitelistEntry(entry.source.toString().toStdString(),
                                    entry.target.scheme().toStdString(),
                                    entry.target.host().toStdString(),
                                    true);
  }
}

void QCefClientHandlerDelegate::DoClose(CefRefPtr<CefBrowser> browser) {
  if (cef_browser_ != nullptr &&
      cef_browser_->GetIdentifier() == browser->GetIdentifier()) {
    cef_browser_ = nullptr;

    // Close web view.
    emit web_page_->windowClosed();
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
    unsigned char* data = (unsigned char*)malloc(image_size);
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

void QCefClientHandlerDelegate::OnLoadStarted(CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame) {
  if (cef_browser_ != nullptr &&
      browser->GetIdentifier() == cef_browser_->GetIdentifier() &&
      browser->GetMainFrame()->GetIdentifier() == frame->GetIdentifier()) {
    emit web_page_->loadStarted();
  }
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

QString QCefClientHandlerDelegate::OnLoadError(
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
  return web_page_->pageErrorContent();
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

  const QString name(message->GetName().ToString().c_str());
  qDebug() << Q_FUNC_INFO << name;
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
    qDebug() << "msg:" << msg;
    const QJsonDocument doc(QJsonDocument::fromJson(msg.toUtf8()));
    qDebug() << "doc: " << doc;
    if (doc.isObject()) {
      web_page_->handleWebMessage(doc.object());
    } else {
      qWarning() << __FUNCTION__ << " invalid json message:" << msg;
    }
    return true;
  }

  // Web Notification.
  if (name == kQCefWebNotificationBody) {
    CefRefPtr<CefListValue> args = message->GetArgumentList();
    if (args->GetSize() < 2) {
      qCritical() << "Invalid web notification body, parameters mismatch";
      return false;
    }
    const QString title = QString::fromStdString(args->GetString(1).ToString());

    QString body;
    QIcon icon;
    if (args->GetSize() > 2) {
      // Parse notification option.
      CefRefPtr<CefDictionaryValue> dict = args->GetDictionary(2);
      CefDictionaryValue::KeyList keys;
      if (dict->GetKeys(keys)) {
        for (const CefString& key : keys) {
          CefString value = dict->GetString(key);
          if (key == "body") {
            body = QString::fromStdString(value.ToString());
          } else if (key == "icon") {
            // TODO(LiuLang): call DownloadImage()
            //icon = QIcon(QString::fromStdString(value.ToString()));
          }
        }
      }
//      web_page_->showNotification(title, body, icon);
//      return true;
    }

    emit web_page_->notificationReceived(title, body);
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

void QCefClientHandlerDelegate::OnPreKeyEvent(const QKeyEvent& event) {
  auto event_delegate = web_page_->getEventDelegate();
  if (event_delegate != nullptr) {
    event_delegate->onPreKeyEvent(event);
  }
}

bool QCefClientHandlerDelegate::OnBeforeBrowse(const CefString& url,
                                               bool is_redirect) {
  auto event_delegate = web_page_->getEventDelegate();
  if (event_delegate != nullptr) {
    const QUrl qUrl(QString::fromStdString(url.ToString()));
    return event_delegate->onBeforeBrowse(qUrl, is_redirect);
  } else {
    return false;
  }
}

void QCefClientHandlerDelegate::OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model) {
  Q_UNUSED(browser);
  Q_UNUSED(frame);
  auto event_delegate = web_page_->getEventDelegate();
  if (event_delegate != nullptr) {
    QCefContextMenuParams qcef_params;
    qcef_params.p_->params = params;
    if (context_menu_ == nullptr) {
      context_menu_ = new QCefContextMenu();
    }
    context_menu_->clear();
    event_delegate->onBeforeContextMenu(web_page_, context_menu_, qcef_params);
    model->Clear();
    for (const QCefContextMenu::MenuItem& item : context_menu_->items()) {
      switch (item.type) {
        case QCefContextMenu::ItemType::Separator: {
          model->AddSeparator();
          break;
        }
        case QCefContextMenu::ItemType::Item: {
          model->AddItem(item.id, item.label.toStdString());
          break;
        }
        default: {
          break;
        }
      }
    }
  }
}

bool QCefClientHandlerDelegate::OnContextMenuCommand(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    int command_id) {
  Q_UNUSED(browser);
  Q_UNUSED(frame);
  Q_UNUSED(params);
  if (context_menu_ != nullptr) {
    if (context_menu_->callbacks().contains(command_id)) {
      context_menu_->callbacks().value(command_id)(web_page_);
      return true;
    }
  }
  return false;
}
