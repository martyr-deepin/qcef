/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "core/qcef_dialog_handler.h"

#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "include/cef_parser.h"

QCefDialogHandler::QCefDialogHandler() {

}

bool QCefDialogHandler::OnFileDialog(
    CefRefPtr<CefBrowser> browser,
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
  } else {
    switch (mode_type) {
      case FILE_DIALOG_OPEN: {
        title_str = QObject::tr("Select File");
        break;
      }
      case FILE_DIALOG_OPEN_MULTIPLE: {
        title_str = QObject::tr("Select Files");
        break;
      }
      case FILE_DIALOG_OPEN_FOLDER: {
        title_str = QObject::tr("Open");
        break;
      }
      case FILE_DIALOG_SAVE: {
        title_str = QObject::tr("Save");
        break;
      }
      default: {
        break;
      }
    }
  }

  QFileDialog* dialog = new QFileDialog();
  dialog->setWindowTitle(title_str);
  dialog->setConfirmOverwrite(mode & FILE_DIALOG_OVERWRITEPROMPT_FLAG);
  const QString path = default_file_path.ToString().c_str();
  QFileInfo info;
  if (path.isEmpty()) {
    info = QDir::homePath();
  } else {
    info = path;
  }
  switch (mode_type) {
    case FILE_DIALOG_OPEN: {
      dialog->setAcceptMode(QFileDialog::AcceptOpen);
      if (info.isFile()) {
        dialog->selectFile(info.absoluteFilePath());
      } else {
        dialog->setDirectory(info.absoluteFilePath());
      }
      dialog->setFileMode(QFileDialog::ExistingFile);
      break;
    }
    case FILE_DIALOG_OPEN_FOLDER: {
      dialog->setAcceptMode(QFileDialog::AcceptOpen);
      if (info.isFile()) {
        dialog->selectFile(info.absoluteFilePath());
      } else {
        dialog->setDirectory(info.absoluteFilePath());
      }
      dialog->setFileMode(QFileDialog::Directory);
      break;
    }
    case FILE_DIALOG_OPEN_MULTIPLE: {
      dialog->setAcceptMode(QFileDialog::AcceptOpen);
      if (info.isFile()) {
        dialog->selectFile(info.absoluteFilePath());
      } else {
        dialog->setDirectory(info.absoluteFilePath());
      }
      dialog->setFileMode(QFileDialog::ExistingFile);
      break;
    }
    case FILE_DIALOG_SAVE: {
      dialog->setAcceptMode(QFileDialog::AcceptSave);
      dialog->setFileMode(QFileDialog::AnyFile);
      if (info.isFile()) {
        dialog->selectFile(info.absoluteFilePath());
      } else {
        dialog->setDirectory(info.absoluteFilePath());
      }
      dialog->selectFile(info.fileName());
      break;
    }
    default: {
      break;
    }
  }

  dialog->show();
  QObject::connect(dialog, &QFileDialog::finished, [=](int ret) {
    std::vector<CefString> selected_files;
    for (const QString& file : dialog->selectedFiles()) {
      selected_files.push_back(file.toStdString());
    }
    dialog->deleteLater();
    if (ret == QDialog::Accepted && !selected_files.empty()) {
      callback->Continue(selected_accept_filter, selected_files);
    } else {
      callback->Cancel();
    }
  });

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
