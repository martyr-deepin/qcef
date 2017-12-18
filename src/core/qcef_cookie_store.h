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

#ifndef QCEF_CORE_QCEF_COOKIE_STORE_H
#define QCEF_CORE_QCEF_COOKIE_STORE_H

#include <QString>
#include <QVariantMap>

#include "qcef_core_export.h"

/**
 * Flush the backing store (if any) to disk.
 */
void QCEF_CORE_EXPORT QCefFlushCookies();

/**
 * Get browser cookie with |name| as cookie name and |domain| as cookie path.
 * This method is used to bypass browser security restriction.
 * Returns cookie value on success, or "" on error.
 * @param domain Domain name of cookie.
 * @param name Cookie name.
 * @return Cookie value on success or an empty string on error.
 */
QString QCEF_CORE_EXPORT QCefGetCookie(const QString& domain,
                                       const QString& name);

/**
 * Get all cookies associated with |domain|.
 * Returns a map of (cookie-name, cookie-value) pair.
 * @param domain Domain name of cookies.
 * @return A map of cookie-name => cookie-value pair.
 */
QVariantMap QCEF_CORE_EXPORT QCefGetCookies(const QString& domain);

/**
 * Movie cookie with |name| from domain |old_domain| to domain |new_domain|.
 * @param old_domain Name of old cookie domain.
 * @param new_domain Name of new cookie domain.
 * @param name Cookie name.
 */
void QCEF_CORE_EXPORT QCefMoveCookie(const QString& old_domain,
                                     const QString& new_domain,
                                     const QString& name);

/**
 * Remove cookie item.
 * @param domain Domain name of cookies.
 * @param name Cookie name to be removed.
 * @return true if this cookie exists and is removed, else false.
 */

bool QCEF_CORE_EXPORT QCefRemoveCookie(const QString& domain,
                                       const QString& name);

/**
 * Set browser cookie at |domain| with |name| and cookie value |value|.
 * @param domain Domain name of cookies.
 * @param name Cookie name to be inserted.
 * @param value Value of cookie.
 */
void QCEF_CORE_EXPORT QCefSetCookie(const QString& domain,
                                    const QString& name,
                                    const QString& value);

#endif  // QCEF_CORE_QCEF_COOKIE_STORE_H
