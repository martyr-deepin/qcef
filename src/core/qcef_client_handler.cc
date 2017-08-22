// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "core/qcef_client_handler.h"

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/base/cef_logging.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

QCefClientHandler::QCefClientHandler(Delegate* delegate) : delegate_(delegate) {

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
  CEF_REQUIRE_UI_THREAD();

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void QCefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  (void) browser;

  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnBeforeClose();
  }
}

void QCefClientHandler::OnLoadStart(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefLoadHandler::TransitionType transition_type) {
  (void) transition_type;
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

void QCefClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& url) {
  (void) browser;
  (void) frame;
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnUrlChanged(url);
  }
}

void QCefClientHandler::OnFaviconURLChange(
    CefRefPtr<CefBrowser> browser,
    const std::vector<CefString>& icon_urls) {
  (void) browser;
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnFaviconURLChange(icon_urls);
  }
}

void QCefClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
                                               bool fullscreen) {
  (void) browser;
  CEF_REQUIRE_UI_THREAD();

  if (delegate_ != nullptr) {
    delegate_->OnSetFullscreen(fullscreen);
  }
}

void QCefClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                      const CefString& title) {
  (void) browser;
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
  (void) browser;
  (void) frame;
  (void) target_frame_name;
  (void) target_disposition;
  (void) user_gesture;
  (void) popupFeatures;
  (void) windowInfo;
  (void) client;
  (void) settings;
  (void) no_javascript_access;

  // TODO(LiuLang): Add option.

  // Do not pop up any window, instead only notify delegate.
  if (delegate_ != nullptr) {
    delegate_->OnBeforePopup(target_url);
  }
  return true;
}