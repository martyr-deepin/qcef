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

#include "widgets/qcef_web_settings.h"

#include <QFontDatabase>

struct QCefWebSettingsPrivate {
  QString standard_font_family =
      QFontDatabase::systemFont(QFontDatabase::GeneralFont).family();
  QString fixed_font_family =
      QFontDatabase::systemFont(QFontDatabase::FixedFont).family();
  QString serif_font_family = "serif";
  QString sans_serif_font_family = "sans";
  QString cursive_font_family;
  QString fantasy_font_family;
  int default_font_size;
  int default_fixed_font_size;
  int minimum_font_size;
  int minimum_logical_font_size;

  QString default_encoding;
  QCefWebSettings::State remote_fonts = QCefWebSettings::StateDefault;
  QCefWebSettings::State javascript = QCefWebSettings::StateDefault;
  QCefWebSettings::State javascript_close_windows =
      QCefWebSettings::StateDefault;
  QCefWebSettings::State javascript_access_clipboard =
      QCefWebSettings::StateDefault;
  QCefWebSettings::State javascript_dom_paste =
      QCefWebSettings::StateDefault;
  QCefWebSettings::State plugin = QCefWebSettings::StateDefault;
  QCefWebSettings::State universal_access_from_file_urls =
      QCefWebSettings::StateDefault;
  QCefWebSettings::State file_access_from_file_urls =
      QCefWebSettings::StateDefault;
  QCefWebSettings::State web_security = QCefWebSettings::StateDefault;
  QCefWebSettings::State image_loading = QCefWebSettings::StateDefault;
  QCefWebSettings::State text_area_resize = QCefWebSettings::StateDefault;
  QCefWebSettings::State local_storage = QCefWebSettings::StateDefault;
  QCefWebSettings::State databases = QCefWebSettings::StateDefault;
  QCefWebSettings::State application_cache = QCefWebSettings::StateDefault;
  QCefWebSettings::State webgl = QCefWebSettings::StateDefault;
  QString accept_language_list = "";
  QCefWebSettings::CrossOriginList cross_origin_list;
};

QCefWebSettings::QCefWebSettings() : p_(new QCefWebSettingsPrivate()) {

}

QCefWebSettings::~QCefWebSettings() {
  delete p_;
  p_ = nullptr;
}

void QCefWebSettings::setDefaultEncoding(const QString& encoding) {
  p_->default_encoding = encoding;
}

QString QCefWebSettings::defaultEncoding() const {
  return p_->default_encoding;
}

void QCefWebSettings::setRemoteFonts(State state) {
  p_->remote_fonts = state;
}

QCefWebSettings::State QCefWebSettings::remoteFonts() const {
  return p_->remote_fonts;
}

void QCefWebSettings::setJavascript(State state) {
  p_->javascript = state;
}

QCefWebSettings::State QCefWebSettings::javascript() const {
  return p_->javascript;
}

void QCefWebSettings::setJavascriptCloseWindow(State state) {
  p_->javascript_close_windows = state;
}

QCefWebSettings::State QCefWebSettings::javascriptCloseWindow() const {
  return p_->javascript_close_windows;
}

void QCefWebSettings::setJavascriptAccessClipboard(State state) {
  p_->javascript_access_clipboard = state;
}

QCefWebSettings::State QCefWebSettings::javascriptAccessClipboard() const {
  return p_->javascript_access_clipboard;
}

void QCefWebSettings::setJavascriptDomPaste(State state) {
  p_->javascript_dom_paste = state;
}

QCefWebSettings::State QCefWebSettings::javascriptDomPaster() const {
  return p_->javascript_dom_paste;
}

void QCefWebSettings::setPlugin(State state) {
  p_->plugin = state;
}

QCefWebSettings::State QCefWebSettings::plugin() const {
  return p_->plugin;
}

void QCefWebSettings::sestUniversalAccessFromFileUrls(State state) {
  p_->universal_access_from_file_urls = state;
}

QCefWebSettings::State QCefWebSettings::universalAccessFromFileUrls() const {
  return p_->universal_access_from_file_urls;
}

void QCefWebSettings::setFileAccessFromFileUrls(State state) {
  p_->file_access_from_file_urls = state;
}

QCefWebSettings::State QCefWebSettings::fileAccessFromFileUrls() const {
  return p_->file_access_from_file_urls;
}

void QCefWebSettings::setWebSecurity(State state) {
  p_->web_security = state;
}

QCefWebSettings::State QCefWebSettings::webSecurity() const {
  return p_->web_security;
}

void QCefWebSettings::setImageLoading(State state) {
  p_->image_loading = state;
}

QCefWebSettings::State QCefWebSettings::imageLoading() const {
  return p_->image_loading;
}

void QCefWebSettings::setTextAreaResize(State state) const {
  p_->text_area_resize = state;
}

QCefWebSettings::State QCefWebSettings::textAreaResize() const {
  return p_->text_area_resize;
}

void QCefWebSettings::setLocalStorage(State state) {
  p_->local_storage = state;
}

QCefWebSettings::State QCefWebSettings::localStorage() const {
  return p_->local_storage;
}

void QCefWebSettings::setDatabases(State state) {
  p_->databases = state;
}

QCefWebSettings::State QCefWebSettings::databases() const {
  return p_->databases;
}

void QCefWebSettings::setApplicationCache(State state) {
  p_->application_cache = state;
}

QCefWebSettings::State QCefWebSettings::applicationCache() const {
  return p_->application_cache;
}

void QCefWebSettings::setWebGL(State state) {
  p_->webgl = state;
}

QCefWebSettings::State QCefWebSettings::webGL() const {
  return p_->webgl;
}

void QCefWebSettings::setAcceptLanguageList(const QString& list) {
  p_->accept_language_list = list;
}

QString QCefWebSettings::acceptLanguageList() const {
  return p_->accept_language_list;
}

void QCefWebSettings::addCrossOriginWhiteEntry(const QUrl& source,
                                               const QUrl& target,
                                               bool allow_subdomains) {
  p_->cross_origin_list.append({source, target, allow_subdomains});
}

const QCefWebSettings::CrossOriginList&
QCefWebSettings::crossOriginWhiteList() const {
  return p_->cross_origin_list;
}

QString QCefWebSettings::standardFontFamily() const {
  return p_->standard_font_family;
}

void QCefWebSettings::setStandardFontFamily(const QString& font) {
  p_->standard_font_family = font;
}

QString QCefWebSettings::fixedFontFamily() const {
  return p_->fixed_font_family;
}

void QCefWebSettings::setFixedFontFamily(const QString& font) {
  p_->fixed_font_family = font;
}

QString QCefWebSettings::serifFontFamily() const {
  return p_->serif_font_family;
}

void QCefWebSettings::setSerifFontFamily(const QString& font) {
  p_->serif_font_family = font;
}

QString QCefWebSettings::sansSerifFontFamily() const {
  return p_->sans_serif_font_family;
}

void QCefWebSettings::setSansSerifFontFamily(const QString& font) {
  p_->sans_serif_font_family = font;
}

QString QCefWebSettings::cursiveFontFamily() const {
  return p_->cursive_font_family;
}

void QCefWebSettings::setCursiveFontFamily(const QString& font) const {
  p_->cursive_font_family = font;
}

QString QCefWebSettings::fantasyFontFamily() const {
  return p_->fantasy_font_family;
}

void QCefWebSettings::setFantasyFontFamily(const QString& font) const {
  p_->fantasy_font_family = font;
}

int QCefWebSettings::defaultFontSize() const {
  return p_->default_font_size;
}

void QCefWebSettings::setDefaultFontSize(int size) {
  p_->default_font_size = size;
}

int QCefWebSettings::defaultFixedFontSize() const {
  return p_->default_fixed_font_size;
}

void QCefWebSettings::setDefaultFixedFontSize(int size) {
  p_->default_fixed_font_size = size;
}

int QCefWebSettings::minimumFontSize() const {
  return p_->minimum_font_size;
}

void QCefWebSettings::setMinimumFontSize(int size) {
  p_->minimum_font_size = size;
}

int QCefWebSettings::minimumLogicalFontSize() const {
  return p_->minimum_logical_font_size;
}

void QCefWebSettings::setMinimumLogicalFontSize(int size) {
  p_->minimum_logical_font_size = size;
}

