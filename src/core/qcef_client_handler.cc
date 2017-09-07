// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "core/qcef_client_handler.h"

#include "core/qcef_dialog_handler.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/wrapper/cef_helpers.h"

namespace {

enum client_menu_ids {
  CLIENT_ID_SHOW_DEVTOOLS = MENU_ID_USER_FIRST,
  CLIENT_ID_CLOSE_DEVTOOLS,
  CLIENT_ID_INSPECT_ELEMENT,
  CLIENT_ID_SHOW_SSL_INFO,
  CLIENT_ID_TESTMENU_SUBMENU,
  CLIENT_ID_TESTMENU_CHECKITEM,
  CLIENT_ID_TESTMENU_RADIOITEM1,
  CLIENT_ID_TESTMENU_RADIOITEM2,
  CLIENT_ID_TESTMENU_RADIOITEM3,
};

}  // namespace

class QCefClientDownloadImageCallback : public CefDownloadImageCallback {
 public:
  explicit QCefClientDownloadImageCallback(
      CefRefPtr<QCefClientHandler> client_handler)
      : client_handler_(client_handler) { }

  void OnDownloadImageFinished(const CefString& image_url,
                               int http_status_code,
                               CefRefPtr<CefImage> image) OVERRIDE {
    if (image != nullptr) {
      client_handler_->NotifyFavicon(image_url, image);
    }
    LOG(ERROR) << "favicon image:" << image;
  }

 private:
  CefRefPtr<QCefClientHandler> client_handler_;

 IMPLEMENT_REFCOUNTING(QCefClientDownloadImageCallback);
  DISALLOW_COPY_AND_ASSIGN(QCefClientDownloadImageCallback);
};


QCefClientHandler::QCefClientHandler(Delegate* delegate)
    : delegate_(delegate),
      dialog_handler_(new QCefDialogHandler()) {
}

QCefClientHandler::~QCefClientHandler() {
}

void QCefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnBrowserCreated(browser);
  }
}

bool QCefClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  (void)browser;
  CEF_REQUIRE_UI_THREAD();

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void QCefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnBeforeClose(browser);
  }
}

void QCefClientHandler::OnLoadStart(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefLoadHandler::TransitionType transition_type) {
  (void)transition_type;
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnLoadStarted(browser, frame);
  }
}

void QCefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    ErrorCode errorCode,
                                    const CefString& errorText,
                                    const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED) {
    return;
  }

  // Display a load error message.
  std::stringstream ss;
  std::string msg;
  if (delegate_ != nullptr) {
    msg = delegate_->OnLoadError(browser, frame, errorCode);
  }
  if (!msg.empty()) {
    ss << msg;
  } else {
    ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
       << std::string(failedUrl) << " with error " << std::string(errorText)
       << " (" << errorCode << ").</h2></body></html>";
  }

  frame->LoadString(ss.str(), failedUrl);
}


void QCefClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                             bool isLoading,
                                             bool canGoBack,
                                             bool canGoForward) {
  if (delegate_ != nullptr) {
    delegate_->OnLoadingStateChange(browser,
                                    isLoading,
                                    canGoBack,
                                    canGoForward);
  }
}


void QCefClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  int httpStatusCode) {
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnLoadEnd(browser, frame, httpStatusCode);
  }
}

bool QCefClientHandler::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) {
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    return delegate_->OnProcessMessageReceived(browser,
                                               source_process,
                                               message);
  } else {
    return CefClient::OnProcessMessageReceived(browser,
                                               source_process,
                                               message);
  }
}

void QCefClientHandler::OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model) {
  LOG(ERROR) << __FUNCTION__;

  (void)browser;
  (void)frame;
  CEF_REQUIRE_UI_THREAD();
  model->AddSeparator();
  model->AddItem(MENU_ID_CUSTOM_FIRST, "Fuck");
}

bool QCefClientHandler::OnContextMenuCommand(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    int command_id,
    CefContextMenuHandler::EventFlags event_flags) {
  LOG(ERROR) << __FUNCTION__;
  CEF_REQUIRE_UI_THREAD();

  return false;
}

CefRefPtr<CefDialogHandler> QCefClientHandler::GetDialogHandler() {
  return dialog_handler_;
}

CefRefPtr<CefJSDialogHandler> QCefClientHandler::GetJSDialogHandler() {
  return dialog_handler_;
}

void QCefClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& url) {
  (void)browser;
  CEF_REQUIRE_UI_THREAD();

  // Only update the address for the main (top-level) frame.
  if (frame->IsMain() && delegate_ != nullptr) {
    delegate_->OnUrlChanged(url);
  }
}

void QCefClientHandler::OnFaviconURLChange(
    CefRefPtr<CefBrowser> browser,
    const std::vector<CefString>& icon_urls) {
  (void)browser;
  CEF_REQUIRE_UI_THREAD();

  if (!icon_urls.empty()) {
    browser->GetHost()->DownloadImage(icon_urls[0], true, 16, false,
                                      new QCefClientDownloadImageCallback(this));
  }
}

void QCefClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
                                               bool fullscreen) {
  (void)browser;
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnSetFullscreen(fullscreen);
  }
}

void QCefClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                      const CefString& title) {
  (void)browser;
  CEF_REQUIRE_UI_THREAD();
  if (delegate_ != nullptr) {
    delegate_->OnTitleChanged(title);
  }
}

bool QCefClientHandler::OnBeforePopup(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& target_url,
    const CefString& target_frame_name,
    CefLifeSpanHandler::WindowOpenDisposition target_disposition,
    bool user_gesture,
    const CefPopupFeatures& popupFeatures,
    CefWindowInfo& windowInfo,
    CefRefPtr<CefClient>& client,
    CefBrowserSettings& settings,
    bool* no_javascript_access) {
  (void)browser;
  (void)frame;
  (void)target_frame_name;
  (void)target_disposition;
  (void)user_gesture;
  (void)popupFeatures;
  (void)windowInfo;
  (void)client;
  (void)settings;
  (void)no_javascript_access;

  // TODO(Deepin Ltd.): Add option.

  // Do not pop up any window, instead only notify delegate.
  if (delegate_ != nullptr) {
    delegate_->OnBeforePopup(target_url);
  }
  return true;
}

void QCefClientHandler::OnGotFocus(CefRefPtr<CefBrowser> browser) {
  if (delegate_ != nullptr) {
    delegate_->OnGotFocus(browser);
  }
}

void QCefClientHandler::NotifyFavicon(const CefString& icon_url,
                                      CefRefPtr<CefImage> icon) {
  if (delegate_ != nullptr) {
    delegate_->OnFaviconURLChange(icon_url, icon);
  }
}

//void QCefClientHandler::showDevTools(CefRefPtr<CefBrowser> browser,
//                                     const CefPoint& inspect_element_at) {
//  if (!CefCurrentlyOn(TID_UI)) {
//    // Execute this method on the UI thread.
////    CefPostTask(TID_UI, base::Bind(QCefClientHandler::showDevTools, this, browser,
////                                   inspect_element_at));
//    return;
//  }
//
//  CefWindowInfo windowInfo;
//  CefRefPtr<CefClient> client;
//  CefBrowserSettings settings;
//
//  CefRefPtr<CefBrowserHost> host = browser->GetHost();
//
//  // Test if the DevTools browser already exists.
//  bool has_devtools = host->HasDevTools();
//
//  if (has_devtools) {
//    // Create the DevTools browser if it doesn't already exist.
//    // Otherwise, focus the existing DevTools browser and inspect the element
//    // at |inspect_element_at| if non-empty.
//    host->ShowDevTools(windowInfo, client, settings, inspect_element_at);
//  }
//}