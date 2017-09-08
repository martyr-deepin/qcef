// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_BROWSER_DEMO_BROWSER_WINDOW_H
#define QCEF_BROWSER_DEMO_BROWSER_WINDOW_H

#include <QFrame>

class QCefWebView;
struct BrowserWindowPrivate;

class BrowserWindow : public QFrame {
  Q_OBJECT

 public:
  explicit BrowserWindow(QWidget* parent = nullptr);
  ~BrowserWindow() override;

 private:
  void initConnections();
  void initUI();

  BrowserWindowPrivate* p_ = nullptr;

 private slots:
  void onAddressEditActivated();
  void onReloadButtonClicked();

  void onFullscreenRequested(bool fullscreen);
  void onLoadingStateChanged(bool is_loading,
                             bool can_go_back,
                             bool can_go_forward);
  void onUrlChanged(const QUrl& url);
};

#endif  // QCEF_BROWSER_DEMO_BROWSER_WINDOW_H
