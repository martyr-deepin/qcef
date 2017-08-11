// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_global_settings.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/file_util.h"

namespace {

const int kPortMin = 1024;
const int kPortMax = 65535;

const char kFlashLibrary[] =
    "/usr/lib/pepperflashplugin-nonfree/libpepflashplayer.so";
const char kFlashManifest[] =
    "/usr/lib/pepperflashplugin-nonfree/manifest.json";

}  // namespace

struct QCefGlobalSettingsPrivate {
  bool single_process = false;
  bool no_sandbox = false;
  bool disable_command_line_args = false;
  QString cache_path = "";
  QString user_data_dir = "";
  QString user_agent = "";
  QString log_file = "";
  QCefGlobalSettings::LogSeverity log_severity =
      QCefGlobalSettings::LogSeverity::Error;
  int remote_debug_port = 9222;
  bool ignore_certificate_errors = false;
  bool remote_debug = false;
  QString accept_language_list = "";
  bool pepper_flash = false;
  QStringList custom_schemes;
  QCefGlobalSettings::CrossOriginList cross_origin_list;
};

QCefGlobalSettings::QCefGlobalSettings()
    : p_(new QCefGlobalSettingsPrivate()) {
  // TODO(LiuLang): Initialize default values.
}

QCefGlobalSettings::~QCefGlobalSettings() {
  delete p_;
  p_ = nullptr;
}

void QCefGlobalSettings::setSingleProcess(bool enabled) {
  p_->single_process = enabled;
}

bool QCefGlobalSettings::singleProcess() const {
  return p_->single_process;
}

void QCefGlobalSettings::setNoSandbox(bool enabled) {
  p_->no_sandbox = enabled;
}

bool QCefGlobalSettings::noSandbox() const {
  return p_->no_sandbox;
}

void QCefGlobalSettings::setDisableCommandLineArgs(bool enabled) {
  p_->disable_command_line_args = enabled;
}

bool QCefGlobalSettings::disableCommandLineArgs() const {
  return p_->disable_command_line_args;
}

void QCefGlobalSettings::setCachePath(const QString& path) {
  p_->cache_path = path;
}

const QString& QCefGlobalSettings::cachePath() const {
  return p_->cache_path;
}

void QCefGlobalSettings::setUserDataPath(const QString& path) {
  p_->user_data_dir = path;
}

const QString& QCefGlobalSettings::userDataPath() const {
  return p_->user_data_dir;
}

void QCefGlobalSettings::setUserAgent(const QString& ua) {
  p_->user_agent = ua;
}

const QString& QCefGlobalSettings::userAgent() const {
  return p_->user_agent;
}

void QCefGlobalSettings::setLogFile(const QString& filepath) {
  p_->log_file = filepath;
}

const QString& QCefGlobalSettings::logFile() const {
  return p_->log_file;
}

void QCefGlobalSettings::setLogSeverity(QCefGlobalSettings::LogSeverity level) {
  p_->log_severity = level;
}

QCefGlobalSettings::LogSeverity QCefGlobalSettings::logSeverity() const {
  return p_->log_severity;
}

void QCefGlobalSettings::setRemoteDebugPort(int port) {
  if (port < kPortMin || port > kPortMax) {
    qCritical() << "setRemoteDebugPort() Ignores invalid port number:" << port;
    return;
  }
  p_->remote_debug_port = port;
}

int QCefGlobalSettings::remoteDebugPort() const {
  return p_->remote_debug_port;
}

void QCefGlobalSettings::setRemoteDebug(bool enabled) {
  p_->remote_debug = enabled;
}

bool QCefGlobalSettings::remoteDebug() const {
  return p_->remote_debug;
}

void QCefGlobalSettings::setIgnoresCertificateErrors(bool enabled) {
  p_->ignore_certificate_errors = enabled;
}

bool QCefGlobalSettings::ignoresCertificateErrors() const {
  return p_->ignore_certificate_errors;
}

void QCefGlobalSettings::setAcceptLanguageList(const QString& list) {
  p_->accept_language_list = list;
}

const QString& QCefGlobalSettings::acceptLanguageList() const {
  return p_->accept_language_list;
}

void QCefGlobalSettings::setPepperFlash(bool enabled) {
  p_->pepper_flash = enabled;
}

bool QCefGlobalSettings::pepperFlash() const {
  return p_->pepper_flash;
}

QString QCefGlobalSettings::getPepperFlashPath() const {
  return kFlashLibrary;
}

QString QCefGlobalSettings::getPepperFlashVersion() const {
  QByteArray content;
  QString version;
  if (ReadRawFile(kFlashManifest, content)) {
    const QJsonObject object = QJsonDocument::fromJson(content).object();
    if (!object.isEmpty()) {
      version = object.value("version").toString("");
    }
  }
  return version;
}

void QCefGlobalSettings::addCustomScheme(const QString& scheme) {
  p_->custom_schemes.append(scheme);
}

const QStringList& QCefGlobalSettings::customSchemes() const {
  return p_->custom_schemes;
}

void QCefGlobalSettings::addCrossOriginWhiteEntry(const QUrl& source,
                                                  const QUrl& target) {
  p_->cross_origin_list.append(qMakePair(source, target));
}

const QCefGlobalSettings::CrossOriginList&
QCefGlobalSettings::crossOriginWhiteList() const {
  return p_->cross_origin_list;
}
