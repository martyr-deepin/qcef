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

#include "core/qcef_scheme_handler_factory.h"

#include <QDebug>
#include <QFile>
#include <QMimeDatabase>
#include <QUrl>
#include <include/wrapper/cef_helpers.h>

namespace {

QString Ext2Mime(const QString& fileName) {
  const auto pos = fileName.lastIndexOf('.');
  const auto ext = fileName.mid(pos + 1).toLower();

  if (ext == "css") {
    return "text/css";
  }
  if (ext == "html") {
    return "text/html";
  }
  if (ext == "js") {
    return "application/javascript";
  }
  if (ext == "less") {
    return "text/less";
  }
  if (ext == "svg") {
    return "image/svg+xml";
  }
  if (ext == "png") {
    return "image/png";
  }
  if (ext == "gif") {
    return "image/gif";
  }
  if (ext == "jpg" || ext == "jpeg") {
    return "image/jpeg";
  }
  return "";
}

CefStreamResourceHandler*
CreateQFileStreamResourceHandler(const QString& path) {
  if (!QFile::exists(path)) {
    qWarning() << __FUNCTION__ << "File not found:" << path;
    return nullptr;
  }
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) {
    qWarning() << __FUNCTION__ << "Failed to open file: " << path;
    return nullptr;
  }

  const QByteArray content = file.readAll();
  QString mime(Ext2Mime(path));
  if (mime.isEmpty()) {
    QMimeDatabase mime_database;
    const QMimeType mime_type = mime_database.mimeTypeForData(content);
    mime = mime_type.name();
  }
  const char* content_bytes = content.constData();
  CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForData(
          static_cast<void*>(const_cast<char*>(content_bytes)),
          (size_t)(file.size()));

  // Set mime type name to empty.
  return new CefStreamResourceHandler(mime.toStdString(), stream);
};

}  // namespace

CefRefPtr<CefResourceHandler> QCefSchemeHandlerFactory::Create(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& scheme_name,
    CefRefPtr<CefRequest> request) {
  (void) browser;
  (void) frame;
  CEF_REQUIRE_IO_THREAD();

  const QString url(QString::fromStdString(request->GetURL().ToString()));
  if (scheme_name == "qrc") {
    // Handle qrc protocol.
    const QString qrc_file(url.mid(3));
    return CreateQFileStreamResourceHandler(qrc_file);
  }
  if (custom_scheme_handler_ != nullptr) {
    const QString filepath = custom_scheme_handler_(QUrl(url));
    return CreateQFileStreamResourceHandler(filepath);
  }
  return nullptr;
}