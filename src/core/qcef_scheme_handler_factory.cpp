// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_scheme_handler_factory.h"

#include <QDebug>
#include <QFile>
#include <QUrl>

namespace {

std::string ext2Mime(const QString& fileName) {
  const auto pos = fileName.lastIndexOf('.');
  const auto ext = fileName.mid(pos + 1);

  // TODO(LiuLang): Add mime type register interface.

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
  qWarning() << "Unknown MIME type for " << fileName;
  return "application/octet-stream";
}

CefStreamResourceHandler* CreateQFileStreamResourceHandler(
    const QString& path) {
  if (!QFile::exists(path)) {
    qWarning() << __FUNCTION__ << "File not found:" << path;
    return nullptr;
  }
  const std::string mimeType = ext2Mime(QUrl(path).fileName());

  QFile qFile(path);
  qFile.open(QIODevice::ReadOnly | QIODevice::Unbuffered);
  const auto content = qFile.readAll();

  const char* contentBytes = content.constData();
  CefRefPtr<CefStreamReader> stream =
      CefStreamReader::CreateForData(
          static_cast<void*>(const_cast<char*>(contentBytes)),
          (size_t)(qFile.size()));
  return new CefStreamResourceHandler(mimeType, stream);
};

}  // namespace

CefRefPtr<CefResourceHandler> QCefSchemeHandlerFactory::Create(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& scheme_name,
    CefRefPtr<CefRequest> request) {
  (void) browser;
  (void) frame;

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

void QCefSchemeHandlerFactory::setCustomSchemeHandler(
    QCefSchemeHandler handler) {
  custom_scheme_handler_ = handler;
}
