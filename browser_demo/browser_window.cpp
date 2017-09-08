// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "browser_window.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWebChannel>

#include <qcef_context.h>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include "browser_tab_widget.h"

struct BrowserWindowPrivate {
  QFrame* address_bar = nullptr;
  QPushButton* back_button = nullptr;
  QPushButton* forward_button = nullptr;
  QPushButton* reload_button = nullptr;
  QPushButton* stop_button = nullptr;
  QLineEdit* address_edit = nullptr;
  BrowserTabWidget* tab_widget = nullptr;
};

BrowserWindow::BrowserWindow(QWidget* parent)
    : QFrame(parent),
      p_(new BrowserWindowPrivate()) {
  this->setObjectName("BrowserWindow");
  this->initUI();
  this->initConnections();
}

BrowserWindow::~BrowserWindow() {
  delete p_;
  p_ = nullptr;
}

//  connect(page, &QCefWebPage::loadFinished, [=]() {
////    page->runJavaScript("console.log('loadFinished');");
//    p_->refresh_button->setEnabled(true);
//    p_->stop_button->setEnabled(false);
//  });

void BrowserWindow::initConnections() {
  connect(p_->address_edit, &QLineEdit::returnPressed,
          this, &BrowserWindow::onAddressEditActivated);

  connect(p_->back_button, &QPushButton::clicked,
          p_->tab_widget, &BrowserTabWidget::back);
  connect(p_->forward_button, &QPushButton::clicked,
          p_->tab_widget, &BrowserTabWidget::forward);
  connect(p_->reload_button, &QPushButton::clicked,
          p_->tab_widget, &BrowserTabWidget::reload);
  connect(p_->stop_button, &QPushButton::clicked,
          p_->tab_widget, &BrowserTabWidget::stop);

  connect(p_->tab_widget, &BrowserTabWidget::fullscreenRequested,
          this, &BrowserWindow::onFullscreenRequested);
  connect(p_->tab_widget, &BrowserTabWidget::urlChanged,
          this, &BrowserWindow::onUrlChanged);
  connect(p_->tab_widget, &BrowserTabWidget::loadingStateChanged,
          this, &BrowserWindow::onLoadingStateChanged);
}

void BrowserWindow::initUI() {
  p_->address_bar = new QFrame(this);
  p_->back_button = new QPushButton("Back", this);
  p_->forward_button = new QPushButton("Forward", this);
  p_->reload_button = new QPushButton("Refresh", this);
  p_->stop_button = new QPushButton("Stop", this);
  p_->address_edit = new QLineEdit(this);

//  p_->web_view = new QCefWebView(this);
//  p_->web_view->page()->webChannel()->registerObject("dialog", this);
//  QCefWebSettings* settings = p_->web_view->page()->settings();
//  settings->addCrossOriginWhiteEntry(QUrl("http://music.163.com"),
//                                     QUrl("http://126.com"),
//                                     true);

  p_->tab_widget = new BrowserTabWidget();
  // Create the first web view.
  p_->tab_widget->createNewBrowser(false);

  QHBoxLayout* toolbar_layout = new QHBoxLayout();
  toolbar_layout->setContentsMargins(0, 0, 0, 0);
  toolbar_layout->setSpacing(0);
  toolbar_layout->addWidget(p_->back_button);
  toolbar_layout->addWidget(p_->forward_button);
  toolbar_layout->addWidget(p_->reload_button);
  toolbar_layout->addWidget(p_->stop_button);
  toolbar_layout->addSpacing(5);
  toolbar_layout->addWidget(p_->address_edit);
  p_->address_bar->setLayout(toolbar_layout);
  p_->back_button->setEnabled(false);
  p_->forward_button->setEnabled(false);
  p_->reload_button->setEnabled(false);
  p_->stop_button->setEnabled(false);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(5);
  layout->addWidget(p_->address_bar, 0);
  layout->addWidget(p_->tab_widget, 100);
  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

void BrowserWindow::onAddressEditActivated() {
  const QString text = p_->address_edit->text();
  if (!text.isEmpty()) {
    const QUrl url(text);
    if (url.isValid()) {
      p_->tab_widget->load(url);
    }
  }
}

void BrowserWindow::onFullscreenRequested(bool fullscreen) {
  if (fullscreen) {
    p_->address_bar->hide();
    this->showFullScreen();
  } else {
    p_->address_bar->show();
    this->showNormal();
  }
}

void BrowserWindow::closeEvent(QCloseEvent* event) {
  QFrame::closeEvent(event);

  // Quit cef message loop on browser window closed.
  QCefQuitLoop();
}

void BrowserWindow::onLoadingStateChanged(bool is_loading,
                                          bool can_go_back,
                                          bool can_go_forward) {
  p_->back_button->setEnabled(can_go_back);
  p_->forward_button->setEnabled(can_go_forward);
  p_->reload_button->setEnabled(!is_loading);
  p_->stop_button->setEnabled(is_loading);
}

void BrowserWindow::onUrlChanged(const QUrl& url) {
  p_->address_edit->setText(url.toDisplayString());
}
