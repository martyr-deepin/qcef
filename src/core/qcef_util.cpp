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