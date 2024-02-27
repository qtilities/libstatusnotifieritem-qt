/*
    SPDX-FileCopyrightText:  2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
*/
#ifndef SNI_QT_PRIVATE_H
#define SNI_QT_PRIVATE_H

#include "statusnotifieritem.h"
#include "statusnotifieritemdbus_p.hpp"

#include <QDBusConnection>
#include <QIcon>
#include <QObject>
#include <QString>

class StatusNotifierItemPrivate : public QObject
{
    Q_OBJECT

public:
    StatusNotifierItemPrivate(StatusNotifierItem* item);
    StatusNotifierItemPrivate() = delete;

    void init(QString id);

#ifdef QT_DBUS_LIB
    SNIIconList iconToPixmapList(const QIcon&);

    StatusNotifierItemDBus* dbus;
    SNIIconList serializedIcon;
    SNIIconList serializedAttentionIcon;
    SNIIconList serializedOverlayIcon;
    SNIIconList serializedToolTipIcon;
#endif
    StatusNotifierItem* q;
    StatusNotifierItem::SNICategory category;
    StatusNotifierItem::SNIStatus   status;
    QString id;
    QString title;

    // icons
    QString iconName,
            overlayIconName,
            attentionIconName;
    QIcon   icon,
            overlayIcon,
            attentionIcon;
    qint64  iconCacheKey,
            overlayIconCacheKey,
            attentionIconCacheKey;

    // tooltip
    QString toolTipTitle,
            toolTipSubTitle,
            toolTipIconName;
    QIcon   toolTipIcon;
    qint64  toolTipIconCacheKey;
};

#endif // SNI_QT_PRIVATE_H
