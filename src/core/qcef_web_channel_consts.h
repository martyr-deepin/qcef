// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef QCEF_CORE_QCEF_WEB_CHANNEL_CONSTS_H
#define QCEF_CORE_QCEF_WEB_CHANNEL_CONSTS_H

// Message names used between browser process and renderer processes.
const char kQCefRenderContextCreated[] = "QCefRenderContextCreated";
const char kQCefRenderContextReleased[] = "QCefRenderContextReleased";
const char kQCefRenderQtMessage[] = "QCefRenderQtMessage";

// Web Notification
const char kQCefWebNotificationRequest[] = "QCefWebNotificationRequest";
const char kQCefWebNotificationBody[] = "QCefWebNotificationBody";

#endif  // QCEF_CORE_QCEF_WEB_CHANNEL_CONSTS_H
