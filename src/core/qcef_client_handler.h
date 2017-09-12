// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef QCEF_CORE_QCEF_CLIENT_HANDLER_H
#define QCEF_CORE_QCEF_CLIENT_HANDLER_H

#include <QKeyEvent>
#include <QString>

#include "core/qcef_dialog_handler.h"
#include "include/cef_client.h"

class QCefClientDownloadImageCallback;

class QCefClientHandler : public CefClient,
                          public CefContextMenuHandler,
                          public CefDisplayHandler,
                          public CefDownloadHandler,
                          public CefFocusHandler,
                          public CefKeyboardHandler,
                          public CefLifeSpanHandler,
                          public CefLoadHandler,
                          public CefRequestHandler {
 public:
  class Delegate {
   public:
    // CefLifeSpanHandler methods
    // Request to popup new browser window to access |target_url|.
    virtual bool OnBeforePopup(
        const CefString& target_url,
        CefLifeSpanHandler::WindowOpenDisposition target_disposition) = 0;

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

    virtual QString OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                int errorCode) = 0;

    virtual bool OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message) = 0;

    // CefDisplayHandler methods
    virtual void OnUrlChanged(const CefString& url) = 0;

    virtual void OnFaviconURLChange(const CefString& icon_url,
                                    CefRefPtr<CefImage> icon) = 0;

    virtual void OnSetFullscreen(bool fullscreen) = 0;

    virtual void OnTitleChanged(const CefString& title) = 0;

    virtual bool OnPreKeyEvent(const QKeyEvent& event) = 0;

    virtual bool OnBeforeBrowse(const CefString& url, bool is_redirect) = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit QCefClientHandler(Delegate* delegate);

  ~QCefClientHandler() override;

  // CefClient methods:
  CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override {
    return this;
  }

  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
    return this;
  }

  CefRefPtr<CefDownloadHandler> GetDownloadHandler() override {
    return this;
  }

  CefRefPtr<CefFocusHandler> GetFocusHandler() override {
    return this;
  }

  CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override {
    return this;
  }

  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
    return this;
  }

  CefRefPtr<CefLoadHandler> GetLoadHandler() override {
    return this;
  }

  CefRefPtr<CefRequestHandler> GetRequestHandler() override {
    return this;
  }

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;

  // CefContextMenuHandler methods:
  void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefContextMenuParams> params,
                           CefRefPtr<CefMenuModel> model) override;

  bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefContextMenuParams> params,
                            int command_id,
                            EventFlags event_flags) override;

  // CefDialogHandler methods:
  CefRefPtr<CefDialogHandler> GetDialogHandler() override;

  CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;

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

  // CefDownloadHandler methods:
  void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefDownloadItem> download_item,
                        const CefString& suggested_name,
                        CefRefPtr<CefBeforeDownloadCallback> callback) override;

  void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefDownloadItem> download_item,
                         CefRefPtr<CefDownloadItemCallback> callback) override;

  // CefFocusHandler methods:
  void OnGotFocus(CefRefPtr<CefBrowser> browser) override;

  // CefKeyboardHandler methods:
  bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                     const CefKeyEvent& event,
                     CefEventHandle os_event,
                     bool* is_keyboard_shortcut) override;

  // CefLifeSpanHandler methods:
  bool OnBeforePopup(
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

  // CefRequestHandler methods:
  bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                      CefRefPtr<CefRequest> request, bool is_redirect) override;

 private:
  void NotifyFavicon(const CefString& icon_url, CefRefPtr<CefImage> icon);

  friend class QCefClientDownloadImageCallback;

  Delegate* delegate_ = nullptr;

  CefRefPtr<QCefDialogHandler> dialog_handler_ = nullptr;

  // Include the default reference counting implementation.
 IMPLEMENT_REFCOUNTING(QCefClientHandler);
  DISALLOW_COPY_AND_ASSIGN(QCefClientHandler);
};

#endif  // QCEF_CORE_QCEF_CLIENT_HANDLER_H
