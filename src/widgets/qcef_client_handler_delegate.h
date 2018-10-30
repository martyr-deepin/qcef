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

#ifndef QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H
#define QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H

#include "core/qcef_client_handler.h"

#include "include/cef_app.h"
#include "include/cef_life_span_handler.h"
#include "widgets/qcef_web_page.h"

class QCefContextMenu;

// Implements delegate of QCefClientHandler, to let QCefWebPage be notified
// about state change of cef browser.
class QCefClientHandlerDelegate : public QCefClientHandler::Delegate
{
public:
    explicit QCefClientHandlerDelegate(QCefWebPage *web_page);

    ~QCefClientHandlerDelegate() override;

    bool OnBeforeBrowse(const CefString &url, bool is_redirect) override;

    void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             CefRefPtr<CefContextMenuParams> params,
                             CefRefPtr<CefMenuModel> model) override;

    bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefContextMenuParams> params,
                              int command_id) override;

    bool OnBeforePopup(
        const CefString &target_url,
        CefLifeSpanHandler::WindowOpenDisposition target_disposition) override;

    void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;

    void DoClose(CefRefPtr<CefBrowser> browser) override;

    void OnFaviconURLChange(const CefString &icon_url,
                            CefRefPtr<CefImage> icon) override;

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

    void OnTitleChanged(const CefString &title) override;

    void OnUrlChanged(const CefString &url) override;

    // Returns true if this key event is grabbed and handled by Qt Application.
    bool OnPreKeyEvent(QKeyEvent *event) override;

    // Update Qt clipboard when cef update its internal clipboard data.
    void OnClipboardChanged(const char *text_data, size_t text_len) override;

    void OnGotFocus(CefRefPtr<CefBrowser> browser) override;

    CefRequestHandler::ReturnValue OnBeforeResourceLoad(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        CefRefPtr<CefRequestCallback> callback) override;

private:
    CefRefPtr<CefBrowser> cef_browser_ = nullptr;
    QCefWebPage *web_page_ = nullptr;
    QCefContextMenu *context_menu_ = nullptr;
};

#endif  // QCEF_WIDGETS_QCEF_CLIENT_HANDLER_DELEGATE_H
