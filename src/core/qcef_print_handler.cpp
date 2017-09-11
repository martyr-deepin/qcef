// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_print_handler.h"


QCefPrintHandler::QCefPrintHandler() {

}

QCefPrintHandler::~QCefPrintHandler() {

}

void QCefPrintHandler::OnPrintStart(CefRefPtr<CefBrowser> browser) {
  (void)browser;
}

void QCefPrintHandler::OnPrintSettings(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefPrintSettings> settings,
                                       bool get_defaults) {
  (void)browser;
  (void)settings;
  (void)get_defaults;
}

bool QCefPrintHandler::OnPrintDialog(
    CefRefPtr<CefBrowser> browser,
    bool has_selection,
    CefRefPtr<CefPrintDialogCallback> callback) {
  (void)browser;
  (void)has_selection;
  (void)callback;
  return false;
}

bool QCefPrintHandler::OnPrintJob(CefRefPtr<CefBrowser> browser,
                                  const CefString& document_name,
                                  const CefString& pdf_file_path,
                                  CefRefPtr<CefPrintJobCallback> callback) {
  (void)browser;
  (void)document_name;
  (void)pdf_file_path;
  (void)callback;
  return false;
}

void QCefPrintHandler::OnPrintReset(CefRefPtr<CefBrowser> browser) {
  (void)browser;
}

CefSize QCefPrintHandler::GetPdfPaperSize(int device_units_per_inch) {
  return CefPrintHandler::GetPdfPaperSize(device_units_per_inch);
}

