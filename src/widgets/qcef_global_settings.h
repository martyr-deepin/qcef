// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_GLOBAL_SETTINGS_H
#define QCEF_WIDGETS_QCEF_GLOBAL_SETTINGS_H

#include <string>

#include "base/qcef_export.h"

struct QCefGlobalSettingsPrivate;

// Setup cef switches during initialization.
class QCEF_EXPORT QCefGlobalSettings {
 public:
  QCefGlobalSettings();
  ~QCefGlobalSettings();

  // Enable single process mode, browser and render process are located in
  // the same process. Default is false.
  void setSingleProcess(bool enabled);
  bool singleProcess() const;

  // Disable sandbox or not for sub-processes, default is false.
  void setNoSandbox(bool enabled);
  bool noSandbox() const;

  // Allows cef to parse command line arguments. Default is true.
  void setDisableCommandLineArgs(bool enabled);
  bool disableCommandLineArgs() const;

  // The location where cache data will be stored on disk. If empty then
  // browsers will be created in "incognito mode" where in-memory caches are
  // used for storage and no data is persisted to disk.
  void setCachePath(const std::string& path);
  std::string cachePath() const;

  // The location where user data such as spell checking dictionary files will
  // be stored on disk. If empty then the default user data directory
  // ("~/.cef_user_data") will be used.
  void setUserDataPath(const std::string& path);
  std::string userDataPath() const;

  // Set User-Agent http header, if |ua| is empty, default value will be used.
  void setUserAgent(const std::string ua);
  std::string userAgent() const;

  // Set absolute path to debug log file.
  void setLogFile(const std::string filepath);
  std::string logFile() const;

  enum class LogSeverity {
    Default,
    Verbose,
    Info,
    Warning,
    Error,
    Disable,
  };
  // Set log level.
  void setLogSeverity(LogSeverity level);
  LogSeverity logSeverity() const;

  // Set remote debug port, default is 9222.
  void setRemoteDebugPort(int port);
  int remoteDebugPort() const;

  // Enable/disable remote debug. Default is false.
  void setRemoteDebug(bool enabled);
  bool remoteDebug() const;

  // Enable this switch to ignore invalid SSl certificates. Default is false.
  void setIgnoresCertificateErrors(bool enabled);
  bool ignoresCertificateErrors() const;

  // Comma delimited ordered list of language codes without any whitespace that
  // will be used in the "Accept-Language" HTTP header. May be overridden on a
  // per-browser basis using the CefBrowserSettings.accept_language_list value.
  // If both values are empty then "en-US,en" will be used.
  void setAcceptLanguageList(const std::string& list);
  std::string acceptLanguageList() const;

  // Enable or disable pepper flash plugin. Default is false.
  // Requires "pepperflashplugin-nonfree" package.
  void setPepperFlash(bool enabled);
  bool pepperFlash() const;
  std::string getPepperFlashPath() const;
  std::string getPepperFlashVersion() const;

 private:
  QCefGlobalSettingsPrivate* p_ = nullptr;
};

#endif  // QCEF_WIDGETS_QCEF_GLOBAL_SETTINGS_H
