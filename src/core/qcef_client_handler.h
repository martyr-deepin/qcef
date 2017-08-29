// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef QCEF_CORE_QCEF_CLIENT_HANDLER_H
#define QCEF_CORE_QCEF_CLIENT_HANDLER_H

#include "include/cef_client.h"

#include <list>

class QCefClientHandler : public CefClient,
                          public CefDisplayHandler,
                          public CefFocusHandler,
                          public CefLifeSpanHandler,
                          public CefLoadHandler {
 public:
  class Delegate {
   public:
    // CefLifeSpanHandler methods
    // Request to popup new browser window to access |target_url|.
    virtual void OnBeforePopup(const CefString& target_url) = 0;

    // Called when the browser is created.
    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) = 0;

    virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) = 0;

    // CefLoadHandler methods
    virtual void OnLoadStarted(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame) = 0;
    virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                      bool isLoading,
                                      bool canGoBack,
                                      bool canGoForward) = 0;
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode) = 0;
    virtual std::string OnLoadError(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    int errorCode) = 0;

    virtual bool OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message) = 0;

    // CefDisplayHandler methods
    virtual void OnUrlChanged(const CefString& url) = 0;
    virtual void OnFaviconURLChange(const std::vector<CefString>& urls) = 0;
    virtual void OnSetFullscreen(bool fullscreen) = 0;
    virtual void OnTitleChanged(const CefString& title) = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit QCefClientHandler(Delegate* delegate);
  ~QCefClientHandler() override ;

  // CefClient methods:
  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
    return this;
  }
  CefRefPtr<CefFocusHandler> GetFocusHandler() override {
    return this;
  }
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
    return this;
  }
  CefRefPtr<CefLoadHandler> GetLoadHandler() override {
    return this;
  }

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;

  // CefDisplayHandler methods:
  void OnAddressChange(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       const CefString& url) override;
  void OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
                          const std::vector<CefString>& icon_urls) override;
  void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
                              bool fullscreen) override;
  void OnTitleChange(CefRefPtr<CefBrowser> browser,
                     const CefString& title) override;

  // CefFocusHandler methods:
  void OnGotFocus(CefRefPtr<CefBrowser> browser) override;

  // CefLifeSpanHandler methods:
  bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     const CefString& target_url,
                     const CefString& target_frame_name,
                     WindowOpenDisposition target_disposition,
                     bool user_gesture,
                     const CefPopupFeatures& popupFeatures,
                     CefWindowInfo& windowInfo,
                     CefRefPtr<CefClient>& client,
                     CefBrowserSettings& settings,
                     bool* no_javascript_access) override;

  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefLoadHandler methods:
  void OnLoadStart(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   TransitionType transition_type) override;
  void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                            bool isLoading,
                            bool canGoBack,
                            bool canGoForward) override;
  void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                 CefRefPtr<CefFrame> frame,
                 int httpStatusCode) override;
  void OnLoadError(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   ErrorCode errorCode,
                   const CefString& errorText,
                   const CefString& failedUrl) override;

 private:
  // Execute Delegate notifications on the main thread.
  // void NotifyFullscreen(bool fullscreen);

  Delegate* delegate_ = nullptr;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(QCefClientHandler);
};

#endif  // QCEF_CORE_QCEF_CLIENT_HANDLER_H
