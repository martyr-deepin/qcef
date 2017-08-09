// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H
#define QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H

#include "include/cef_app.h"

#include "core/qcef_client_handler.h"
#include "core/qcef_util.h"
#include "widgets/qcef_web_page.h"

// Implements delegate of QCefClientHandler, to let QCefWebPage be notified
// about state change of cef browser.
class QCefClientHandlerDelegate : public QCefClientHandler::Delegate {
 public:
  explicit QCefClientHandlerDelegate(QCefWebPage* web_page);
  ~QCefClientHandlerDelegate() override;

  void OnBeforePopup(const CefString& target_url) override;
  void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose() override;
  void OnFaviconURLChange(const std::vector<CefString>& urls) override;
  void OnLoadStarted() override;
  void OnLoadEnd(int httpStatusCode) override;
  std::string OnLoadError(int errorCode) override;
  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;
  void OnSetFullscreen(bool fullscreen) override;
  void OnTitleChanged(const CefString& title) override;
  void OnUrlChanged(const CefString& url) override;

  CefRefPtr<CefBrowser> cef_browser() const { return cef_browser_; }

 private:
  CefRefPtr<CefBrowser> cef_browser_ = nullptr;
  QCefWebPage* web_page_ = nullptr;
};

#endif  // QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H
