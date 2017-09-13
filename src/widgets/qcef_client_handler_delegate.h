// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H
#define QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H

#include "core/qcef_client_handler.h"

#include "include/cef_app.h"
#include "include/cef_life_span_handler.h"
#include "widgets/qcef_web_page.h"

class QCefContextMenu;

// Implements delegate of QCefClientHandler, to let QCefWebPage be notified
// about state change of cef browser.
class QCefClientHandlerDelegate : public QCefClientHandler::Delegate {
 public:
  explicit QCefClientHandlerDelegate(QCefWebPage* web_page);

  ~QCefClientHandlerDelegate() override;

  bool OnBeforeBrowse(const CefString& url, bool is_redirect) override;

  void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefContextMenuParams> params,
                           CefRefPtr<CefMenuModel> model) override;

  bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefContextMenuParams> params,
                            int command_id) override;

  bool OnBeforePopup(
      const CefString& target_url,
      CefLifeSpanHandler::WindowOpenDisposition target_disposition) override;

  void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;

  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  void OnFaviconURLChange(const CefString& icon_url,
                          CefRefPtr<CefImage> icon) override;

  void OnGotFocus(CefRefPtr<CefBrowser> browser) override;

  void OnLoadStarted(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame) override;

  void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                            bool is_loading,
                            bool can_go_back,
                            bool can_go_forward) override;

  void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                 CefRefPtr<CefFrame> frame,
                 int httpStatusCode) override;

  QString OnLoadError(CefRefPtr<CefBrowser> browser,
                      CefRefPtr<CefFrame> frame,
                      int errorCode) override;

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;

  void OnSetFullscreen(bool fullscreen) override;

  void OnTitleChanged(const CefString& title) override;

  void OnUrlChanged(const CefString& url) override;

  bool OnPreKeyEvent(const QKeyEvent& event) override;

 private:
  CefRefPtr<CefBrowser> cef_browser_ = nullptr;
  QCefWebPage* web_page_ = nullptr;
  QCefContextMenu* context_menu_ = nullptr;
};

#endif  // QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H
