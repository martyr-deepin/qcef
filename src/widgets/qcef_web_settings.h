// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_WIDGETS_QCEF_WEB_SETTINGS_H
#define QCEF_WIDGETS_QCEF_WEB_SETTINGS_H

#include <QString>
#include <QUrl>
#include <QVector>

#include "qcef_widgets_export.h"

struct QCefWebSettingsPrivate;

// Settings used for each web page.
class QCEF_WIDGETS_EXPORT QCefWebSettings {
 public:
  QCefWebSettings();
  ~QCefWebSettings();

  enum State {
    StateDefault = 0,
    StateEnabled,
    StateDisabled,
  };

  // Default encoding for Web content. If empty "ISO-8859-1" will be used.
  void setDefaultEncoding(const QString& encoding);
  QString defaultEncoding() const;

  // Controls the loading of fonts from remote sources.
  void setRemoteFonts(State state);
  State remoteFonts() const;

  // Controls whether JavaScript can be executed.
  void setJavascript(State state);
  State javascript() const;

  // Controls whether JavaScript can be used to close windows that were not
  // opened via JavaScript. JavaScript can still be used to close windows that
  // were opened via JavaScript or that have no back/forward history.
  void setJavascriptCloseWindow(State state);
  State javascriptCloseWindow() const;

  // Controls whether JavaScript can access the clipboard.
  void setJavascriptAccessClipboard(State state);
  State javascriptAccessClipboard() const;

  // Controls whether DOM pasting is supported in the editor via
  // execCommand("paste"). The |javascript_access_clipboard| setting must also
  // be enabled.
  void setJavascriptDomPaste(State state);
  State javascriptDomPaster() const;

  // Controls whether any plugins will be loaded.
  void setPlugin(State state);
  State plugin() const;

  // Controls whether file URLs will have access to all URLs.
  void sestUniversalAccessFromFileUrls(State state);
  State universalAccessFromFileUrls() const;

  // Controls whether file URLs will have access to other file URLs.
  void setFileAccessFromFileUrls(State state);
  State fileAccessFromFileUrls() const;

  // Controls whether web security restrictions (same-origin policy) will be
  // enforced. Disabling this setting is not recommend as it will allow risky
  // security behavior such as cross-site scripting (XSS).
  void setWebSecurity(State state);
  State webSecurity() const;

  // Controls whether image URLs will be loaded from the network. A cached image
  // will still be rendered if requested.
  void setImageLoading(State state);
  State imageLoading() const;

  // Controls whether text areas can be resized.
  void setTextAreaResize(State state) const;
  State textAreaResize() const;

  // Controls whether local storage can be used.
  void setLocalStorage(State state);
  State localStorage() const;

  // Controls whether databases can be used.
  void setDatabases(State state);
  State databases() const;

  // Controls whether the application cache can be used.
  void setApplicationCache(State state);
  State applicationCache() const;

  // Controls whether WebGL can be used. Note that WebGL requires hardware
  // support and may not work on all systems even when enabled.
  void setWebGL(State state);
  State webGL() const;

  // Comma delimited ordered list of language codes without any whitespace that
  // will be used in the "Accept-Language" HTTP header. May be set globally
  // using the CefBrowserSettings.accept_language_list value. If both values are
  // empty then "en-US,en" will be used.
  void setAcceptLanguageList(const QString& list);
  QString acceptLanguageList() const;

  // Decline cross-origin limitation from |source| url to |target| url.
  void addCrossOriginWhiteEntry(const QUrl& source,
                                const QUrl& target,
                                bool allow_subdomains);
  struct CrossOriginEntry {
    QUrl source;
    QUrl target;
    bool allow_subdomains;
  };
  typedef QVector<CrossOriginEntry> CrossOriginList;
  const CrossOriginList& crossOriginWhiteList() const;

 private:
  QCefWebSettingsPrivate* p_ = nullptr;
};

#endif  // QCEF_WIDGETS_QCEF_WEB_SETTINGS_H
