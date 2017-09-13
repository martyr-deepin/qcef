// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_string_visitor.h"

StringVisitor::StringVisitor(StringVisitorCallback callback)
    : callback_(std::move(callback)) {
}

void StringVisitor::Visit(const CefString& string) {
  if (callback_ != nullptr) {
    callback_(QString::fromStdString(string.ToString()));
  }
}
