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

#include "core/qcef_global_settings.h"

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
  bool persist_session_cookies = false;
  bool persist_user_preferences = false;
  QString user_agent = "";
  QString log_file = "";
  QCefGlobalSettings::LogSeverity log_severity =
      QCefGlobalSettings::LogSeverity::Error;
  int remote_debug_port = 9222;
  bool ignore_certificate_errors = false;
  bool remote_debug = false;
  QString accept_language_list = "";
  bool pepper_flash = false;
  QList<QUrl> custom_schemes;

  QCefSchemeHandler custom_scheme_handler = nullptr;
  QCefSyncMethodMap sync_method_handlers;

  QCefUserScriptList register_scripts;

  QCefGlobalSettings::ProxyType proxy_type =
      QCefGlobalSettings::ProxyType::Default;
  QString proxy_info;

  QCefCommandLineSwitchList switch_list;

  QString override_path{};

  // Read current locale.
  QString locale = QLocale().name();

  unsigned int background_color = 0xffffffff;
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

void QCefGlobalSettings::setPersistSessionCcookies(bool enabled) {
  p_->persist_session_cookies = enabled;
}

bool QCefGlobalSettings::persistSessionCookies() const {
  return p_->persist_session_cookies;
}

void QCefGlobalSettings::setPersistUserPreferences(bool enabled) {
  p_->persist_user_preferences = enabled;
}

bool QCefGlobalSettings::persistUserPreferences() const {
  return p_->persist_user_preferences;
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

void QCefGlobalSettings::addCustomScheme(const QUrl& url) {
  p_->custom_schemes.append(url);
}

const QList<QUrl>& QCefGlobalSettings::customSchemes() const {
  return p_->custom_schemes;
}

void
QCefGlobalSettings::setCustomSchemeHandler(QCefSchemeHandler handler) {
  p_->custom_scheme_handler = handler;
}

const QCefSchemeHandler& QCefGlobalSettings::getCustomSchemeHandler() const {
  return p_->custom_scheme_handler;
}

void QCefGlobalSettings::registerSyncMethod(const QString& name,
                                            QCefSyncMethod handler) {
  p_->sync_method_handlers.insert(name, handler);
}

const QCefSyncMethodMap& QCefGlobalSettings::getSyncMethods() const {
  return p_->sync_method_handlers;
}

void QCefGlobalSettings::setNoProxy() {
  p_->proxy_type = ProxyType::NoProxy;
  p_->proxy_info = "";
}

void QCefGlobalSettings::setProxyPacUrl(const QUrl& url) {
  p_->proxy_type = ProxyType::PacUrl;
  p_->proxy_info = url.toString();
}

void QCefGlobalSettings::setAutoDetectProxy() {
  p_->proxy_type = ProxyType::AutoDetect;
  p_->proxy_info = "";
}

void QCefGlobalSettings::setProxyServer(const QString& server) {
  p_->proxy_type = ProxyType::ProxyServer;
  p_->proxy_info = server;
}

QCefGlobalSettings::ProxyType QCefGlobalSettings::proxyType() const {
  return p_->proxy_type;
}

const QString& QCefGlobalSettings::proxyInfo() const {
  return p_->proxy_info;
}

void QCefGlobalSettings::registerUserScript(const QString& path,
                                            const QUrl& url) {
  p_->register_scripts.append({path, url});
}

const QCefUserScriptList& QCefGlobalSettings::getUserScripts() const {
  return p_->register_scripts;
}

void QCefGlobalSettings::addCommandLineSwitch(const QString& key,
                                              const QString& value) {
  p_->switch_list.append({key, value});
}

const QCefCommandLineSwitchList&
QCefGlobalSettings::getCommandLineSwitches() const {
  return p_->switch_list;
}

void QCefGlobalSettings::setOverridePath(const QString& path) {
  p_->override_path = path;
}

const QString& QCefGlobalSettings::getOverridePath() const {
  return p_->override_path;
}

void QCefGlobalSettings::setLocale(const QString& locale) {
  p_->locale = locale;
}

QString QCefGlobalSettings::locale() const {
  const QStringList kReveredLocales = {
      "en_GB",
      "en_US",
      "es_419",
      "pt_BR",
      "pt_PT",
      "zh_CN",
      "zh_TW",
  };
  if (kReveredLocales.indexOf(p_->locale) > -1) {
    // Remap locale names to match with chromium's
    return p_->locale.replace('_', '-');
  } else {
    // Remove suffix of locale.
    const int index = p_->locale.indexOf('_');
    return p_->locale.left(index);
  }
}

void QCefGlobalSettings::setBackgroundColor(unsigned int color)
{
    p_->background_color = color;
}

unsigned int QCefGlobalSettings::backgroundColor() const
{
  return p_->background_color;
}
