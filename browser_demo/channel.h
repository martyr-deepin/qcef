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

#ifndef QCEF_BROWSWER_DEMO_CHANNEL_H
#define QCEF_BROWSWER_DEMO_CHANNEL_H

#include <QObject>

class Channel : public QObject {
  Q_OBJECT
 public:
  explicit Channel(QObject* parent = nullptr);

 public slots:
  void printMessage(const QString& msg);
  int count();

  void execSql(const QString& id, const QString& statement);

 signals:
  void onExecSqlDone(const QString& id, int state, const QVariantList& result);

 private:
  int count_ = 0;
};


#endif  // QCEF_BROWSWER_DEMO_CHANNEL_H
