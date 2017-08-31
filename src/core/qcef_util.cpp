// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_util.h"

#include <sys/utsname.h>
#include <string>

bool IsX86Architecture() {
  struct utsname info;
  if (uname(&info) == 0) {
    const std::string machine(info.machine);
    if (machine == "i386" || machine == "i468" || machine == "i586" ||
        machine == "i686" || machine == "x86" ||
        machine == "amd64" || machine == "x86_64") {
      return true;
    }
  }
  return false;
}