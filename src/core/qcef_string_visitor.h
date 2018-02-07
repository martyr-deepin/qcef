/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef QCEF_CORE_QCEF_STRING_VISITOR_H
#define QCEF_CORE_QCEF_STRING_VISITOR_H

#include "include/cef_string_visitor.h"

#include <functional>
#include <QString>
#include <utility>

typedef std::function<void(const QString& value)> StringVisitorCallback ;

// Retrieve cef string asynchronously.
class StringVisitor : public CefStringVisitor {
 public:
  explicit StringVisitor(StringVisitorCallback callback);

  void Visit(const CefString& string) override;

 private:
  IMPLEMENT_REFCOUNTING(StringVisitor);
  DISALLOW_COPY_AND_ASSIGN(StringVisitor);

  StringVisitorCallback callback_ = nullptr;
};

#endif  // QCEF_CORE_QCEF_STRING_VISITOR_H
