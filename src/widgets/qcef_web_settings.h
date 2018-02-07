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

#ifndef QCEF_WIDGETS_QCEF_WEB_SETTINGS_H
#define QCEF_WIDGETS_QCEF_WEB_SETTINGS_H

#include <QString>
#include <QUrl>
#include <QVector>

#include "qcef_widgets_export.h"

struct QCefWebSettingsPrivate;

/**
 * Settings used for each web page.
 */
class QCEF_WIDGETS_EXPORT QCefWebSettings {
 public:
  QCefWebSettings();
  ~QCefWebSettings();

  enum State {
    StateDefault = 0,
    StateEnabled,
    StateDisabled,
  };

  /**
   * Default encoding for Web content. If empty "ISO-8859-1" will be used.
   * @param encoding
   */
  void setDefaultEncoding(const QString& encoding);
  QString defaultEncoding() const;

  /**
   * Controls the loading of fonts from remote sources.
   * @param state
   */
  void setRemoteFonts(State state);
  State remoteFonts() const;

  /**
   * Controls whether JavaScript can be executed.
   * @param state
   */
  void setJavascript(State state);
  State javascript() const;

  /**
   * Controls whether JavaScript can be used to close windows that were not
   * opened via JavaScript. JavaScript can still be used to close windows that
   * were opened via JavaScript or that have no back/forward history.
   * @param state
   */
  void setJavascriptCloseWindow(State state);
  State javascriptCloseWindow() const;

  /**
   * Controls whether JavaScript can access the clipboard.
   * @param state
   */
  void setJavascriptAccessClipboard(State state);
  State javascriptAccessClipboard() const;

  /**
   * Controls whether DOM pasting is supported in the editor via
   * execCommand("paste"). The |javascript_access_clipboard| setting must also
   * be enabled.
   * @param state
   */
  void setJavascriptDomPaste(State state);
  State javascriptDomPaster() const;

  /**
   * Controls whether any plugins will be loaded.
   * @param state
   */
  void setPlugin(State state);
  State plugin() const;

  /**
   * Controls whether file URLs will have access to all URLs.
   * @param state
   */
  void sestUniversalAccessFromFileUrls(State state);
  State universalAccessFromFileUrls() const;

  /**
   * Controls whether file URLs will have access to other file URLs.
   * @param state
   */
  void setFileAccessFromFileUrls(State state);
  State fileAccessFromFileUrls() const;

  /**
   * Controls whether web security restrictions (same-origin policy) will be
   * enforced. Disabling this setting is not recommend as it will allow risky
   * security behavior such as cross-site scripting (XSS).
   * @param state
   */
  void setWebSecurity(State state);
  State webSecurity() const;

  /**
   * Controls whether image URLs will be loaded from the network. A cached image
   * will still be rendered if requested.
   * @param state
   */
  void setImageLoading(State state);
  State imageLoading() const;

  /**
   * Controls whether text areas can be resized.
   * @param state
   */
  void setTextAreaResize(State state) const;
  State textAreaResize() const;

  /**
   * Controls whether local storage can be used.
   * @param state
   */
  void setLocalStorage(State state);
  State localStorage() const;

  /**
   * Controls whether databases can be used.
   * @param state
   */
  void setDatabases(State state);
  State databases() const;

  /**
   * Controls whether the application cache can be used.
   * @param state
   */
  void setApplicationCache(State state);
  State applicationCache() const;

  /**
   * Controls whether WebGL can be used. Note that WebGL requires hardware
   * support and may not work on all systems even when enabled.
   * @param state
   */
  void setWebGL(State state);
  State webGL() const;

  /**
   * Comma delimited ordered list of language codes without any whitespace that
   * will be used in the "Accept-Language" HTTP header. May be set globally
   * using the CefBrowserSettings.accept_language_list value. If both values are
   * empty then "en-US,en" will be used.
   * @param list
   */
  void setAcceptLanguageList(const QString& list);
  QString acceptLanguageList() const;

  /**
   * Decline cross-origin limitation from |source| url to |target| url.
   * NOTE(LiuLang): Currently, cross origin white list instance is invalid,
   * Use setWebSecurity() instead.
   * @param source
   * @param target
   * @param allow_subdomains
   */
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

  /**
   * Font settings.
   * @return
   */
  QString standardFontFamily() const;

  void setStandardFontFamily(const QString& font);
  QString fixedFontFamily() const;
  void setFixedFontFamily(const QString& font);
  QString serifFontFamily() const;
  void setSerifFontFamily(const QString& font);
  QString sansSerifFontFamily() const;
  void setSansSerifFontFamily(const QString& font);
  QString cursiveFontFamily() const;
  void setCursiveFontFamily(const QString& font) const;
  QString fantasyFontFamily() const;
  void setFantasyFontFamily(const QString& font) const;
  int defaultFontSize() const;
  void setDefaultFontSize(int size);
  int defaultFixedFontSize() const;
  void setDefaultFixedFontSize(int size);
  int minimumFontSize() const;
  void setMinimumFontSize(int size);
  int minimumLogicalFontSize() const;
  void setMinimumLogicalFontSize(int size);

 private:
  QCefWebSettingsPrivate* p_ = nullptr;
};

#endif  // QCEF_WIDGETS_QCEF_WEB_SETTINGS_H
