// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_RENDERER_HANDLER_H
#define QCEF_CORE_QCEF_RENDERER_HANDLER_H

#include "include/cef_render_process_handler.h"

#include "core/qcef_sync_method.h"

// Handle messages in renderer processes.
class QCefRendererHandler : public CefRenderProcessHandler {
 public:
  QCefRendererHandler() { }
  explicit QCefRendererHandler(const QCefSyncMethodMap& map);
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

  QCefSyncMethodMap sync_methods_;
};

#endif  // QCEF_CORE_QCEF_RENDERER_HANDLER_H
