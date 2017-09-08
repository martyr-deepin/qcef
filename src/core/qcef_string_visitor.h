// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_STRING_VISITOR_H
#define QCEF_CORE_QCEF_STRING_VISITOR_H

#include "include/cef_string_visitor.h"

#include <QString>

typedef void(*StringVisitorCallback)(const QString& value) ;

// Retrieve cef string asynchronously.
class StringVisitor : public CefStringVisitor {
 public:
  StringVisitor(StringVisitorCallback callback) : callback_(callback) {
  }

  void Visit(const CefString& string) override;

 private:
  IMPLEMENT_REFCOUNTING(StringVisitor);
  DISALLOW_COPY_AND_ASSIGN(StringVisitor);

  StringVisitorCallback callback_ = nullptr;
};

#endif  // QCEF_CORE_QCEF_STRING_VISITOR_H
