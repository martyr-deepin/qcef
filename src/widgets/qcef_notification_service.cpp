// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "widgets/qcef_notification_service.h"

#include <QIcon>
#include <QPixmap>

namespace {

const char kDBusService[] = "org.freedesktop.Notifications";
const char kDBusPath[] = "/org/freedesktop/Notifications";
const char kDBusInterface[] = "org.freedesktop.Notifications";

const char kNotify[] = "Notify";

// This is a raw data image format which describes the width,
// height, rowstride, has alpha, bits per sample, channels and
// image data respectively.
//struct ImageData {
//  qint32 width;
//  qint32 height;
//  qint32 rowstride;
//  bool has_alpha;
//  qint32 bits_per_sample;
//  qint32 channels;
//  QByteArray data;
//};

//Q_DECLARE_METATYPE(ImageData)

}  // namespace

QCefNotificationService::QCefNotificationService(QObject* parent)
    : QDBusAbstractInterface(kDBusService,
                             kDBusPath,
                             kDBusInterface,
                             QDBusConnection::sessionBus(),
                             parent) {
//  qDBusRegisterMetaType<ImageData>();
}

void QCefNotificationService::notify(const QString& title,
                                     const QString& body,
                                     const QIcon& icon) {
  //  STRING app_name;
  //  UINT32 replaces_id;
  //  STRING app_icon;
  //  STRING summary;
  //  STRING body;
  //  ARRAY actions;
  //  DICT hints;
  //  INT32 expire_timeout;
  const QString app_name("App name");
  const quint32 replaced_id = 0;
//  const QString app_icon("/tmp/firefox.png");
  const QString app_icon;
  const QStringList actions = {"default", "Activate"};
  QVariantMap hints;
  // This is a raw data image format which describes the width,
  // height, rowstride, has alpha, bits per sample, channels and
  // image data respectively.
  QVariantList image_data;
  QImage image(icon.pixmap(QSize(32, 32)).toImage());
  image_data.append(image.width());
  image_data.append(image.height());
  // rowstrike.
  image_data.append(256);
  image_data.append(image.hasAlphaChannel());
  // Bits per sample.
  image_data.append(8);
  int channels = 3;
  if (image.isGrayscale()) {
    channels = 1;
  } else if (image.hasAlphaChannel()) {
    channels = 4;
  }
  image_data.append(channels);
  const QByteArray bytes((const char*)(image.constBits()),
                         image.width() * image.height());
  image_data.append(bytes);

  hints.insert("imdage-data", image_data);
  const qint32 expire_timeout = -1;
  this->asyncCall(kNotify, app_name, replaced_id, app_icon,
                  title, body,
                  actions, hints, expire_timeout);
}