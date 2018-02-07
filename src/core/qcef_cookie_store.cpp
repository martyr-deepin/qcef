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

#include "core/qcef_cookie_store.h"

#include <unistd.h>

#include <string>
#include <QSemaphore>
#include <QUrl>

#include "include/cef_cookie.h"

namespace {

const int kSleepInterval = 100;
const int kMaximumAcquireCount = 300;

// Used by getCookie().
class CookieVisitor : public CefCookieVisitor {
 public:
  CookieVisitor(const std::string& name)
      : CefCookieVisitor(),
        cookie_name(name),
        cookie_value(),
        semaphore(0) { }
  ~CookieVisitor() override { }

  bool Visit(const CefCookie& cookie,
             int count,
             int total,
             bool& deleteCookie) override;

  std::string cookie_name;
  std::string cookie_value;
  QSemaphore semaphore;

 private:
 IMPLEMENT_REFCOUNTING(CookieVisitor);
};

// Used by getCookies().
class CookieTotalVisitor : public CefCookieVisitor {
 public:
  CookieTotalVisitor() : CefCookieVisitor(), cookies(), semaphore(0) { }
  ~CookieTotalVisitor() override { }

  bool Visit(const CefCookie& cookie,
             int count,
             int total,
             bool& deleteCookie) override;

  QMap<QString, QString> cookies;
  QSemaphore semaphore;

 private:
 IMPLEMENT_REFCOUNTING(CookieTotalVisitor);
};

bool CookieVisitor::Visit(const CefCookie& cookie,
                          int count,
                          int total,
                          bool& deleteCookie) {
  Q_UNUSED(count);
  Q_UNUSED(total);

  if (cookie_name == std::string(CefString(&cookie.name))) {
    cookie_value = std::string(CefString(&cookie.value));
    return false;
  }
  return true;
}

bool CookieTotalVisitor::Visit(const CefCookie& cookie,
                               int count,
                               int total,
                               bool& deleteCookie) {
  cookies.insert(QString::fromStdString(std::string(CefString(&cookie.name))),
                 QString::fromStdString(std::string(CefString(&cookie.value))));

  if (count + 1 == total) {
    semaphore.release();
  }

  return true;
}

}  // namespace

void QCefFlushCookies() {
  auto cookie_manager = CefCookieManager::GetGlobalManager(nullptr);
  cookie_manager->FlushStore(nullptr);
}

QString QCefGetCookie(const QString& domain, const QString& name) {
  CefRefPtr<CookieVisitor> cookie_visitor =
      new CookieVisitor(name.toStdString());

  auto cookie_manager = CefCookieManager::GetGlobalManager(nullptr);
  cookie_manager->VisitUrlCookies(domain.toStdString(), false,
                                  CefRefPtr<CefCookieVisitor>(cookie_visitor));

  // Wait until visitor release.
  int count = 0;
  while (!cookie_visitor->semaphore.tryAcquire() &&
         count < kMaximumAcquireCount) {
    usleep(kSleepInterval);
    count++;
  }

  return QString::fromStdString(cookie_visitor->cookie_value);
}

QVariantMap QCefGetCookies(const QString& domain) {
  CefRefPtr<CookieTotalVisitor> cookie_visitor = new CookieTotalVisitor();
  auto cookie_manager = CefCookieManager::GetGlobalManager(nullptr);
  cookie_manager->VisitUrlCookies(domain.toStdString(), false,
                                  CefRefPtr<CefCookieVisitor>(cookie_visitor));

  // Wait until visitor release.
  int count = 0;
  while (!cookie_visitor->semaphore.tryAcquire() &&
         count < kMaximumAcquireCount) {
    usleep(kSleepInterval);
    count++;
  }

  QVariantMap map;

  for (auto iter = cookie_visitor->cookies.cbegin();
       iter != cookie_visitor->cookies.cend(); ++iter) {
    map.insert(iter.key(), iter.value());
  }

  return map;
}

void QCefMoveCookie(const QString& old_domain,
                    const QString& new_domain,
                    const QString& name) {
  Q_UNUSED(old_domain);
  Q_UNUSED(new_domain);
  Q_UNUSED(name);
  // TODO(LiuLang):
}

bool QCefRemoveCookie(const QString& domain, const QString& name) {
  auto cookie_manager = CefCookieManager::GetGlobalManager(nullptr);
  cookie_manager->DeleteCookies(domain.toStdString(),
                                name.toStdString(),
                                nullptr);
  cookie_manager->FlushStore(nullptr);
  return true;
}

void QCefSetCookie(const QString& domain,
                   const QString& name,
                   const QString& value) {
  auto cookie_manager = CefCookieManager::GetGlobalManager(nullptr);
  CefCookie cefCookie;
  CefString(&cefCookie.name) = name.toStdString();
  CefString(&cefCookie.domain) = QUrl(domain).host().toStdString();
  CefString(&cefCookie.value) = value.toStdString();
  cookie_manager->SetCookie(domain.toStdString(), cefCookie, nullptr);
  cookie_manager->FlushStore(nullptr);
}
