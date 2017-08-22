// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_COOKIE_STORE_H
#define QCEF_CORE_QCEF_COOKIE_STORE_H

#include <QString>
#include <QVariantMap>

#include "qcef_core_export.h"

// Flush the backing store (if any) to disk.
void FlushCookies();

// Get browser cookie with |name| as cookie name and |domain| as cookie path.
// This method is used to bypass browser security restriction.
// Returns cookie value on success, or "" on error.
QString QCEF_CORE_EXPORT GetCookie(const QString& domain, const QString& name);

// Get all cookies associated with |domain|.
// Returns a map of (cookie-name, cookie-value) pair.
QVariantMap QCEF_CORE_EXPORT GetCookies(const QString& domain);

// Movie cookie with |name| from domain |old_domain| to domain |new_domain|.
void QCEF_CORE_EXPORT MoveCookie(const QString& old_domain,
                                 const QString& new_domain,
                                 const QString& name);

// Remove cookie with |name| at |domain|.
// Returns true if this cookie exists and is removed, else false.
bool QCEF_CORE_EXPORT RemoveCookie(const QString& domain, const QString& name);

// Set browser cookie at |domain| with |name| and cookie value |value|.
void QCEF_CORE_EXPORT SetCookie(const QString& domain,
                                const QString& name,
                                const QString& value);

#endif  // QCEF_CORE_QCEF_COOKIE_STORE_H
