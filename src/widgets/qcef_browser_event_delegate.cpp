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

#include "widgets/qcef_browser_event_delegate.h"

#include "widgets/qcef_browser_event_delegate_p.h"

QCefContextMenuParams::QCefContextMenuParams()
    : p_(new QCefContextMenuParamsPrivate()) {
}

QCefContextMenuParams::~QCefContextMenuParams() {
  if (p_ != nullptr) {
    delete p_;
    p_ = nullptr;
  }
}

int QCefContextMenuParams::getXCoord() const {
  return p_->params->GetXCoord();
}

int QCefContextMenuParams::getYCoord() const {
  return p_->params->GetYCoord();
}

QCefContextMenuParams::TypeFlags QCefContextMenuParams::getTypeFlags() const {
  return static_cast<QCefContextMenuFlags>(p_->params->GetTypeFlags());
}

QString QCefContextMenuParams::getLinkUrl() const {
  return QString::fromStdString(p_->params->GetLinkUrl());
}

QString QCefContextMenuParams::getUnfilteredLinkUrl() const {
  return QString::fromStdString(p_->params->GetUnfilteredLinkUrl());
}

QString QCefContextMenuParams::getSourceUrl() const {
  return QString::fromStdString(p_->params->GetSourceUrl());
}

bool QCefContextMenuParams::hasImageContents() const {
  return p_->params->HasImageContents();
}

QString QCefContextMenuParams::getTitleText() const {
  return QString::fromStdString(p_->params->GetTitleText());
}

QString QCefContextMenuParams::getPageUrl() const {
  return QString::fromStdString(p_->params->GetPageUrl());
}

QString QCefContextMenuParams::getFrameUrl() const {
  return QString::fromStdString(p_->params->GetFrameUrl());
}

QString QCefContextMenuParams::getFrameCharset() const {
  return QString::fromStdString(p_->params->GetFrameCharset());
}

QCefContextMenuParams::MediaType QCefContextMenuParams::getMediaType() const {
  return static_cast<QCefContextMenuMediaTypeFlags>(p_->params->GetMediaType());
}

QCefContextMenuParams::MediaStateFlags
QCefContextMenuParams::getMediaStateFlags() const {
  return static_cast<QCefContextMenuMediaFlags>(
      p_->params->GetMediaStateFlags());
}

QString QCefContextMenuParams::getSelectionText() const {
  return QString::fromStdString(p_->params->GetSelectionText().ToString());
}

bool QCefContextMenuParams::isEditable() const {
  return p_->params->IsEditable();
}

QCefContextMenuParams::EditStateFlags
QCefContextMenuParams::getEditStateFlags() const {
  return static_cast<QCefContextMenuEditFlags>(p_->params->GetEditStateFlags());
}

QCefContextMenu::QCefContextMenu() : items_(), callbacks_() {

}

void QCefContextMenu::addItem(int id,
                              const QString& label,
                              bool enabled,
                              Callback callback) {
  items_.append(MenuItem{ItemType::Item, id, label, enabled});
  callbacks_.insert(id, callback);
}

void QCefContextMenu::addSeparator() {
  items_.append(MenuItem{ItemType::Separator});
}

void QCefContextMenu::clear() {
  items_.clear();
  callbacks_.clear();
}

const QVector<QCefContextMenu::MenuItem> QCefContextMenu::items() const {
  return items_;
}

const QMap<int, QCefContextMenu::Callback> QCefContextMenu::callbacks() const {
  return callbacks_;
}
