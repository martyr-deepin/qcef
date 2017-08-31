// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_DIALOG_HANDLER_H
#define QCEF_CORE_QCEF_DIALOG_HANDLER_H

#include "include/cef_dialog_handler.h"
#include "include/cef_jsdialog_handler.h"

class QCefDialogHandler : public CefDialogHandler,
                          public CefJSDialogHandler {
 public:
  QCefDialogHandler();

  bool OnFileDialog(CefRefPtr<CefBrowser> browser, FileDialogMode mode,
                    const CefString& title, const CefString& default_file_path,
                    const std::vector<CefString>& accept_filters,
                    int selected_accept_filter,
                    CefRefPtr<CefFileDialogCallback> callback) override;

  bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                  const CefString& origin_url,
                  JSDialogType dialog_type,
                  const CefString& message_text,
                  const CefString& default_prompt_text,
                  CefRefPtr<CefJSDialogCallback> callback,
                  bool& suppress_message) override;

  bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
                            const CefString& message_text,
                            bool is_reload,
                            CefRefPtr<CefJSDialogCallback> callback) override;

  void OnResetDialogState(CefRefPtr<CefBrowser> browser) override;

  void OnDialogClosed(CefRefPtr<CefBrowser> browser) override;

 private:
  IMPLEMENT_REFCOUNTING(QCefDialogHandler);
};


#endif  // QCEF_CORE_QCEF_DIALOG_HANDLER_H
