/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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
