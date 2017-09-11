// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_PRINT_HANDLER_H
#define QCEF_CORE_QCEF_PRINT_HANDLER_H

#include "include/cef_print_handler.h"

class QCefPrintHandler : public CefPrintHandler {
 public:
  QCefPrintHandler();
  ~QCefPrintHandler() override;
  void OnPrintStart(CefRefPtr<CefBrowser> browser) override;

  void OnPrintSettings(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefPrintSettings> settings,
                       bool get_defaults) override;

  bool OnPrintDialog(CefRefPtr<CefBrowser> browser, bool has_selection,
                     CefRefPtr<CefPrintDialogCallback> callback) override;

  bool OnPrintJob(CefRefPtr<CefBrowser> browser, const CefString& document_name,
                  const CefString& pdf_file_path,
                  CefRefPtr<CefPrintJobCallback> callback) override;

  void OnPrintReset(CefRefPtr<CefBrowser> browser) override;

  CefSize GetPdfPaperSize(int device_units_per_inch) override;

 private:
  IMPLEMENT_REFCOUNTING(QCefPrintHandler);
  DISALLOW_COPY_AND_ASSIGN(QCefPrintHandler);
};


#endif  // QCEF_CORE_QCEF_PRINT_HANDLER_H
