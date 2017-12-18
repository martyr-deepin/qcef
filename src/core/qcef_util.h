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

#ifndef QCEF_CORE_QCEF_UTIL_H
#define QCEF_CORE_QCEF_UTIL_H

#include "qcef_core_export.h"

/**
 * Check whether current cpu architecture is x86 serials.
 * @return Returns true if current cpu type is x86 or x86_64.
 */
QCEF_CORE_EXPORT bool IsX86Architecture();

#endif  // QCEF_CORE_QCEF_UTIL_H
