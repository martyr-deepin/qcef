// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_web_settings.h"

struct QCefWebSettingsPrivate {
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