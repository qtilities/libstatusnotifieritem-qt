/*
    SPDX-FileCopyrightText:  2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
    Author: Andrea Zanellato <redtid3@gmail.com>
*/
#ifndef SNI_QT_PRIVATE_H
#define SNI_QT_PRIVATE_H

#include "statusnotifieritem.h"
#include "dbustypes.h"

#include <QDBusConnection>
#include <QString>

QT_BEGIN_NAMESPACE
class QIcon;
class QMenu;
QT_END_NAMESPACE

class DBusMenuExporter;
class StatusNotifierItemAdaptor;

class StatusNotifierItemPrivate : public QObject
{
public:
    StatusNotifierItemPrivate(StatusNotifierItem* item);
    StatusNotifierItemPrivate() = delete;

    void init(QString id);
    void registerToHost();
    IconPixmapList iconToPixmapList(const QIcon &);

    StatusNotifierItem* q;
    StatusNotifierItemAdaptor* adaptor;

    StatusNotifierItem::ItemCategory category;
    StatusNotifierItem::ItemStatus   status;
    QString        id;
    QString        title;
    QString        service;

    // icons
    QString        iconName,
                   overlayIconName,
                   attentionIconName;
    IconPixmapList icon,
                   overlayIcon,
                   attentionIcon;
    qint64         iconCacheKey,
                   overlayIconCacheKey,
                   attentionIconCacheKey;

    // tooltip
    QString        tooltipTitle,
                   tooltipSubtitle,
                   tooltipIconName;
    IconPixmapList tooltipIcon;
    qint64         tooltipIconCacheKey;

    // menu
    QMenu*            menu            { nullptr };
    QDBusObjectPath   menuObjectPath;
    DBusMenuExporter* menuExporter    { nullptr };
    std::unique_ptr<QDBusConnection> sessionBus;

    static int serviceCounter;

public Q_SLOTS:
    void onMenuDestroyed();
    void onServiceOwnerChanged(
        const QString &service, const QString &oldOwner, const QString &newOwner
    );
};

#endif // SNI_QT_PRIVATE_H
