// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "browser_window.h"

#include <QApplication>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWebChannel>

#include <qcef_context.h>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

struct BrowserWindowPrivate {
  QFrame* address_bar = nullptr;
  QPushButton* back_button = nullptr;
  QPushButton* forward_button = nullptr;
  QPushButton* refresh_button = nullptr;
  QPushButton* stop_button = nullptr;
  QLineEdit* address_edit = nullptr;
  QCefWebView* web_view = nullptr;
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

void BrowserWindow::load(const QUrl& url) {
  p_->web_view->load(url);
}

void BrowserWindow::printMessage(const QString& msg) {
  qDebug() << "print message:" << msg;
  this->setWindowTitle(msg);
}

void BrowserWindow::initConnections() {
  connect(p_->address_edit, &QLineEdit::returnPressed,
          this, &BrowserWindow::onAddressEditActivated);
  QCefWebPage* page = p_->web_view->page();
  connect(page, &QCefWebPage::fullscreenRequested,
          this, &BrowserWindow::onFullscreenRequested);
  connect(page, &QCefWebPage::titleChanged,
          this, &BrowserWindow::setWindowTitle);
  connect(page, &QCefWebPage::urlChanged,
          this, &BrowserWindow::onUrlChanged);
  connect(page, &QCefWebPage::loadStarted, [=]() {
    page->runJavaScript("console.log('loadStarted');");
  });
  connect(page, &QCefWebPage::loadingStateChanged, [=](bool is_loading,
                                                       bool can_go_back,
                                                       bool can_go_forward) {
    const QString script =
        QString("console.log('loadingStateChanged, %1 %2 %3');")
            .arg(static_cast<int>(is_loading))
            .arg(static_cast<int>(can_go_back))
            .arg(static_cast<int>(can_go_forward));
    page->runJavaScript(script);
    p_->refresh_button->setEnabled(!is_loading);
    p_->stop_button->setEnabled(is_loading);
    p_->back_button->setEnabled(can_go_back);
    p_->forward_button->setEnabled(can_go_forward);
    qDebug() << "state changed:" << is_loading << can_go_back << can_go_forward;
  });
  connect(page, &QCefWebPage::loadFinished, [=]() {
    page->runJavaScript("console.log('loadFinished');");
    p_->refresh_button->setEnabled(true);
    p_->stop_button->setEnabled(false);
  });
  connect(page, &QCefWebPage::iconUrlChanged, [=](const QUrl& iconUrl) {
    qDebug() << "icon url changed:" << iconUrl;
  });

  connect(p_->back_button, &QPushButton::clicked,
          page, &QCefWebPage::back);
  connect(p_->forward_button, &QPushButton::clicked,
          page, &QCefWebPage::forward);
  connect(p_->refresh_button, &QPushButton::clicked,
          page, &QCefWebPage::reload);
  connect(p_->stop_button, &QPushButton::clicked,
          page, &QCefWebPage::stop);
}

void BrowserWindow::initUI() {
  p_->address_bar = new QFrame(this);
  p_->back_button = new QPushButton("Back", this);
  p_->forward_button = new QPushButton("Forward", this);
  p_->refresh_button = new QPushButton("Refresh", this);
  p_->stop_button = new QPushButton("Stop", this);
  p_->address_edit = new QLineEdit(this);

  p_->web_view = new QCefWebView(this);
  QCefWebSettings* settings = p_->web_view->page()->settings();
  settings->addCrossOriginWhiteEntry(QUrl("http://music.163.com"),
                                     QUrl("http://126.com"),
                                     true);

  QHBoxLayout* toolbar_layout = new QHBoxLayout();
  toolbar_layout->setContentsMargins(0, 0, 0, 0);
  toolbar_layout->setSpacing(0);
  toolbar_layout->addWidget(p_->back_button);
  toolbar_layout->addWidget(p_->forward_button);
  toolbar_layout->addWidget(p_->refresh_button);
  toolbar_layout->addWidget(p_->stop_button);
  toolbar_layout->addSpacing(5);
  toolbar_layout->addWidget(p_->address_edit);
  p_->address_bar->setLayout(toolbar_layout);
  p_->back_button->setEnabled(false);
  p_->forward_button->setEnabled(false);
  p_->refresh_button->setEnabled(false);
  p_->stop_button->setEnabled(false);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(5);
  layout->addWidget(p_->address_bar, 0);
  layout->addWidget(p_->web_view, 1);
  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

void BrowserWindow::onAddressEditActivated() {
  const QString text = p_->address_edit->text();
  if (!text.isEmpty()) {
    p_->web_view->setUrl(QUrl(text));
    p_->web_view->page()->webChannel()->registerObject("dialog", this);
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
  QWidget::closeEvent(event);

  // Quit cef message loop on browser window closed.
  QCefQuitLoop();
}

void BrowserWindow::onUrlChanged(const QUrl& url) {
  p_->address_edit->setText(url.toDisplayString());
}
