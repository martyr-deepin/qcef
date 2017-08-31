// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_BROWSER_DEMO_BROWSER_WINDOW_H
#define QCEF_BROWSER_DEMO_BROWSER_WINDOW_H

#include <QFrame>

struct BrowserWindowPrivate;

class BrowserWindow : public QFrame {
  Q_OBJECT

 public:
  explicit BrowserWindow(QWidget* parent = nullptr);
  ~BrowserWindow() override;

  void load(const QUrl& url);

 signals:
  void onTimeout();

 public slots:
   void printMessage(const QString& msg);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  void initConnections();
  void initUI();

  BrowserWindowPrivate* p_ = nullptr;

 private slots:
  void onAddressEditActivated();
  void onFullscreenRequested(bool fullscreen);
  void onUrlChanged(const QUrl& url);
};

#endif  // QCEF_BROWSER_DEMO_BROWSER_WINDOW_H
