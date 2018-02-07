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

