// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_scheme_handler_factory.h"

#include <QFile>
#include <QUrl>

namespace {
//
//std::string ext2Mime(const QString& fileName) {
//  const auto pos = fileName.lastIndexOf('.');
//  const auto ext = fileName.right(pos + 1);
//
//  if (ext == "css") {
//    return "text/css";
//  } else if (ext == "html") {
//    return "text/html";
//  } else if (ext == "js") {
//    return "application/javascript";
//  } else if (ext == "less") {
//    return "text/less";
//  } else if (ext == "svg") {
//    return "image/svg+xml";
//  } else if (ext == "png") {
//    return "image/png";
//  } else if (ext == "gif") {
//    return "image/gif";
//  } else if (ext == "jpg" || ext == "jpeg") {
//    return "image/jpeg";
//  } else {
//    //LOG(ERROR) << "Unknown MIME type for " << fileName;
//    return "application/octet-stream";
//  }
//}
//
//CefStreamResourceHandler*
//CreateQFileStreamResourceHandler(const QString& path) {
//  if (!QFile::exists(path)) {
//    return nullptr;
//  }
//  const std::string mimeType = ext2Mime(QUrl(path).fileName());
//
//  QFile qFile(path);
//  qFile.open(QIODevice::ReadOnly | QIODevice::Unbuffered);
//  const auto content = qFile.readAll();
//
//  const char* contentBytes = content.constData();
//  CefRefPtr<CefStreamReader> stream =
//      CefStreamReader::CreateForData(
//          static_cast<void*>(const_cast<char*>(contentBytes)),
//          (size_t)(qFile.size()));
//  return new CefStreamResourceHandler(mimeType, stream);
//};

}  // namespace

CefRefPtr<CefResourceHandler>
QCefSchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 const CefString& scheme_name,
                                 CefRefPtr<CefRequest> request) {

  return nullptr;
}
