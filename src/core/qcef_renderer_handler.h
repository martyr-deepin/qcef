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

#ifndef QCEF_CORE_QCEF_RENDERER_HANDLER_H
#define QCEF_CORE_QCEF_RENDERER_HANDLER_H

#include "core/qcef_global_settings.h"
#include "core/qcef_sync_method.h"
#include "include/cef_render_process_handler.h"

// Handle messages in renderer processes.
class QCefRendererHandler : public CefRenderProcessHandler {
 public:
  QCefRendererHandler() { }
  explicit QCefRendererHandler(const QCefSyncMethodMap& map,
                               const QCefUserScriptList& scripts);
  void OnContextCreated(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefFrame> frame,
                        CefRefPtr<CefV8Context> context) override;

  void OnContextReleased(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         CefRefPtr<CefV8Context> context) override;

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;

  void OnWebKitInitialized() override;

 private:
  IMPLEMENT_REFCOUNTING(QCefRendererHandler);
  DISALLOW_COPY_AND_ASSIGN(QCefRendererHandler);

  QCefSyncMethodMap sync_methods_;
  QList<QCefUserScript> register_scripts_;
};

#endif  // QCEF_CORE_QCEF_RENDERER_HANDLER_H
