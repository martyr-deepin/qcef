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

#include "channel.h"

#include <QDebug>

Channel::Channel(QObject* parent) : QObject(parent) {
  this->setObjectName("channel");
}

void Channel::printMessage(const QString& msg) {
  qDebug() << "Channel::printMessage()" << msg;
}

int Channel::count() {
  count_ ++;
  return count_;
}

void Channel::execSql(const QString& id, const QString& statement) {
  qDebug() << "execSql()" << id << statement;
  emit this->onExecSqlDone(id, 0, QVariantList({"hello", "world"}));
}
