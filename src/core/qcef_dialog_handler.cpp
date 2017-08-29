// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "core/qcef_dialog_handler.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "include/cef_parser.h"

QCefDialogHandler::QCefDialogHandler() {

}

bool QCefDialogHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                     CefDialogHandler::FileDialogMode mode,
                                     const CefString& title,
                                     const CefString& default_file_path,
                                     const std::vector<CefString>& accept_filters,
                                     int selected_accept_filter,
                                     CefRefPtr<CefFileDialogCallback> callback) {
  (void)browser;

  // Remove any modifier flags.
  FileDialogMode mode_type =
      static_cast<FileDialogMode>(mode & FILE_DIALOG_TYPE_MASK);

  QString title_str;
  if (!title.empty()) {
    title_str = QString::fromStdString(title.ToString());
  }

  bool success;
  std::vector<CefString> files;

  if (mode_type == FILE_DIALOG_OPEN) {
    const QString filename =
        QFileDialog::getOpenFileName(nullptr, title_str,
                                     "", "", nullptr);
    files.push_back(filename.toStdString());
    success = (! filename.isEmpty());
  } else if (mode_type == FILE_DIALOG_OPEN_MULTIPLE) {
    const QStringList filename =
        QFileDialog::getOpenFileNames(nullptr, title_str, "", "", nullptr);
    for (int i = 0; i < filename.size(); ++i) {
      files.push_back(filename.at(i).toStdString());
    }
    success = (filename.size() != 0);
  } else if (mode_type == FILE_DIALOG_OPEN_FOLDER) {
    const QString dir =
        QFileDialog::getExistingDirectory(nullptr, title_str, "");
    if (!dir.isEmpty()) {
      files.push_back(dir.toStdString());
    }
    success = (! dir.isEmpty());
  } else if (mode_type == FILE_DIALOG_SAVE) {
    const QString filename =
        QFileDialog::getSaveFileName(nullptr, title_str, "", "", nullptr);
    if (!filename.isEmpty()) {
      files.push_back(filename.toStdString());
    }
    success = (! filename.isEmpty());
  } else {
    return false;
  }

  int filter_index = selected_accept_filter;

  if (success) {
    callback->Continue(filter_index, files);
  } else {
    callback->Cancel();
  }

  return true;

}

bool QCefDialogHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                   const CefString& origin_url,
                                   CefJSDialogHandler::JSDialogType dialog_type,
                                   const CefString& message_text,
                                   const CefString& default_prompt_text,
                                   CefRefPtr<CefJSDialogCallback> callback,
                                   bool& suppress_message) {
  (void)browser;
  const QString text = QString::fromStdString(message_text.ToString());
  const QString title = QString::fromStdString(
      CefFormatUrlForSecurityDisplay(origin_url).ToString());
  switch (dialog_type) {
    case JSDIALOGTYPE_ALERT: {
      QMessageBox* box = new QMessageBox();
      box->setWindowTitle(title);
      box->setText(text);
      box->show();
      callback->Continue(true, CefString());
      break;
    }
    case JSDIALOGTYPE_CONFIRM: {
      QMessageBox* box = new QMessageBox();
      box->setWindowTitle(title);
      box->setText(text);
      box->addButton(QMessageBox::Yes);
      box->addButton(QMessageBox::No);
      box->show();
      QObject::connect(box, &QMessageBox::finished, [=](int ret) {
        callback->Continue(ret == QMessageBox::Yes, CefString());
        box->deleteLater();
      });
      break;
    }
    case JSDIALOGTYPE_PROMPT: {
      QInputDialog* dialog = new QInputDialog();
      dialog->setWindowTitle(title);
      dialog->setLabelText(text);
      const QString default_value =
          QString::fromStdString(default_prompt_text.ToString());
      dialog->setTextValue(default_value);
      dialog->show();
      QObject::connect(dialog, &QDialog::finished, [=](int ret) {
        callback->Continue(ret == QDialog::Accepted,
                           dialog->textValue().toStdString());
        dialog->deleteLater();
      });
      break;
    }
  }

  return true;
}

bool QCefDialogHandler::OnBeforeUnloadDialog(
    CefRefPtr<CefBrowser> browser,
    const CefString& message_text,
    bool is_reload,
    CefRefPtr<CefJSDialogCallback> callback) {
  (void)browser;
  (void)message_text;
  (void)is_reload;
  (void)callback;
  return true;
}

void QCefDialogHandler::OnResetDialogState(CefRefPtr<CefBrowser> browser) {
  CefJSDialogHandler::OnResetDialogState(browser);
}

void QCefDialogHandler::OnDialogClosed(CefRefPtr<CefBrowser> browser) {
  CefJSDialogHandler::OnDialogClosed(browser);
}
