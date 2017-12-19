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

#include "browser_window.h"

#include <QtWidgets/QAction>
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QShortcut>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWebChannel>

#include <qcef_context.h>
#include <qcef_web_page.h>

#include "browser_tab_bar.h"
#include "browser_tab_widget.h"
#include "browser_address_edit.h"

namespace {

const char kBackIcon[] = ":/images/go-previous-symbolic.svg";
const char kForwardIcon[] = ":/images/go-next-symbolic.svg";
const char kReloadIcon[] = ":/images/view-refresh-symbolic.svg";
const char kStopIcon[] = ":/images/window-close-symbolic.svg";
const char kHomeIcon[] = ":/images/user-home-symbolic.svg";
const char kSettingsIcon[] = ":/images/view-more-symbolic.svg";

const char kBackTip[] = "Click to go back";
const char kForwardTip[] = "Click to go forward";
const char kReloadTip[] = "Reload this page";
const char kStopTip[] = "Stop loading this page";
const char kHomeTip[] = "Show home page";
const char kSettingsTip[] = "Customized and control browser";

const char kHomePage[] = "qrc://resources/index.html";

}  // namespace

struct BrowserWindowPrivate {
  QFrame* address_bar = nullptr;
  QPushButton* back_button = nullptr;
  QPushButton* forward_button = nullptr;
  QPushButton* reload_button = nullptr;
  QPushButton* home_button = nullptr;
  QPushButton* settings_button = nullptr;
  BrowserAddressEdit* address_edit = nullptr;
  BrowserTabWidget* tab_widget = nullptr;
  QIcon reload_icon;
  QIcon stop_icon;
  QMenu* settings_menu = nullptr;
  QAction* new_tab = nullptr;
  QAction* zoom_in = nullptr;
  QAction* zoom_reset = nullptr;
  QAction* zoom_out = nullptr;
  QAction* print = nullptr;

  void updateReloadButtonState(bool is_loading);
};

void BrowserWindowPrivate::updateReloadButtonState(bool is_loading) {
  if (is_loading) {
    reload_button->setIcon(stop_icon);
    reload_button->setToolTip(kStopTip);
  } else {
    reload_button->setIcon(reload_icon);
    reload_button->setToolTip(kReloadTip);
  }
}

BrowserWindow::BrowserWindow(QWidget* parent)
    : QFrame(parent),
      p_(new BrowserWindowPrivate()) {
  this->setObjectName("BrowserWindow");
  this->initUI();
  this->initConnections();

  QShortcut* t = new QShortcut(QKeySequence("Ctrl+Alt+A"), this);
  connect(t, &QShortcut::activated, []() {
    qDebug() << "Ctrl+Alt+A activated";
  });
  
  p_->address_edit->setFocus();
}

BrowserWindow::~BrowserWindow() {
  delete p_;
  p_ = nullptr;
}

void BrowserWindow::initConnections() {
  connect(p_->address_edit, &QLineEdit::returnPressed,
          this, &BrowserWindow::onAddressEditActivated);

  connect(p_->back_button, &QPushButton::clicked,
          p_->tab_widget, &BrowserTabWidget::back);
  connect(p_->forward_button, &QPushButton::clicked,
          p_->tab_widget, &BrowserTabWidget::forward);
  connect(p_->reload_button, &QPushButton::clicked,
          this, &BrowserWindow::onReloadButtonClicked);
  connect(p_->home_button, &QPushButton::clicked,
          this, &BrowserWindow::showHomePage);

  connect(p_->tab_widget, &BrowserTabWidget::fullscreenRequested,
          this, &BrowserWindow::onFullscreenRequested);
  connect(p_->tab_widget, &BrowserTabWidget::urlChanged,
          this, &BrowserWindow::onUrlChanged);
  connect(p_->tab_widget, &BrowserTabWidget::loadingStateChanged,
          this, &BrowserWindow::onLoadingStateChanged);

  connect(p_->new_tab, &QAction::triggered, [=]() {
    p_->tab_widget->createNewBrowser(false);
  });
  connect(p_->zoom_in, &QAction::triggered,
          p_->tab_widget, &BrowserTabWidget::zoomIn);
  connect(p_->zoom_out, &QAction::triggered,
          p_->tab_widget, &BrowserTabWidget::zoomOut);
  connect(p_->zoom_reset, &QAction::triggered,
          p_->tab_widget, &BrowserTabWidget::zoomReset);
}

void BrowserWindow::initUI() {
  p_->address_bar = new QFrame(this);
  p_->back_button = new QPushButton(QIcon(kBackIcon), "", this);
  p_->back_button->setFlat(true);
  const QSize button_size(32, 32);
  p_->back_button->setFixedSize(button_size);
  p_->back_button->setToolTip(kBackTip);
  p_->forward_button = new QPushButton(QIcon(kForwardIcon), "", this);
  p_->forward_button->setToolTip(kForwardTip);
  p_->forward_button->setFlat(true);
  p_->forward_button->setFixedSize(button_size);
  p_->reload_icon = QIcon(kReloadIcon);
  p_->stop_icon = QIcon(kStopIcon);
  p_->reload_button = new QPushButton(p_->reload_icon, "", this);
  p_->reload_button->setToolTip(kReloadTip);
  p_->reload_button->setFlat(true);
  p_->reload_button->setFixedSize(button_size);
  p_->home_button = new QPushButton(QIcon(kHomeIcon), "", this);
  p_->home_button->setFlat(true);
  p_->home_button->setToolTip(kHomeTip);
  p_->home_button->setFixedSize(button_size);

  p_->address_edit = new BrowserAddressEdit(this);
  p_->settings_button = new QPushButton(QIcon(kSettingsIcon), "", this);
  // TODO(LiuLang): Remove drop-down arrow.
  p_->settings_button->setFlat(true);
  p_->settings_button->setFixedSize(button_size);
  p_->settings_button->setToolTip(kSettingsTip);
  p_->settings_menu = new QMenu(this);
  p_->new_tab = new QAction("New tab", this);
  p_->zoom_in = new QAction("Zoom in", this);
  p_->zoom_reset = new QAction("Reset zoom", this);
  p_->zoom_out = new QAction("Zoom out", this);
  p_->print = new QAction("Print..", this);
  p_->settings_menu->addAction(p_->new_tab);
  p_->settings_menu->addAction(p_->zoom_in);
  p_->settings_menu->addAction(p_->zoom_reset);
  p_->settings_menu->addAction(p_->zoom_out);
  p_->settings_menu->addAction(p_->print);
  p_->settings_button->setMenu(p_->settings_menu);

  p_->tab_widget = new BrowserTabWidget();
  // Create the first web view.
  p_->tab_widget->createNewBrowser(false);

  QHBoxLayout* toolbar_layout = new QHBoxLayout();
  toolbar_layout->setContentsMargins(0, 0, 0, 0);
  toolbar_layout->setSpacing(0);
  toolbar_layout->addWidget(p_->back_button);
  toolbar_layout->addWidget(p_->forward_button);
  toolbar_layout->addWidget(p_->reload_button);
  toolbar_layout->addWidget(p_->home_button);
  toolbar_layout->addSpacing(5);
  toolbar_layout->addWidget(p_->address_edit);
  toolbar_layout->addSpacing(5);
  toolbar_layout->addWidget(p_->settings_button);
  p_->address_bar->setLayout(toolbar_layout);
  p_->back_button->setEnabled(false);
  p_->forward_button->setEnabled(false);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
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

void BrowserWindow::onReloadButtonClicked() {
  const bool is_loading = p_->tab_widget->isLoading();
  p_->updateReloadButtonState(is_loading);
  if (is_loading) {
    p_->tab_widget->stop();
  } else {
    p_->tab_widget->reload();
  }
}

void BrowserWindow::onLoadingStateChanged(bool is_loading,
                                          bool can_go_back,
                                          bool can_go_forward) {
  p_->back_button->setEnabled(can_go_back);
  p_->forward_button->setEnabled(can_go_forward);
  p_->updateReloadButtonState(is_loading);
  const QCefSSLStatus ssl_status = p_->tab_widget->getSSlStatus();
  const bool secure = ssl_status.is_secure_connection &&
      (ssl_status.content_status == QCefSSLContentStatus::NORMAL_CONTENT);
  p_->address_edit->updateCertificatesInfo(secure);
}

void BrowserWindow::onUrlChanged(const QUrl& url) {
  p_->address_edit->setText(url.toDisplayString());
}

void BrowserWindow::showHomePage() {
  p_->tab_widget->load(QUrl(kHomePage));
}