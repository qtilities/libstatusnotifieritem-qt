/*
    SPDX-FileCopyrightText:  2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
*/
#pragma once

#include <QObject>
#include <QDBusConnection>
#include <QDBusObjectPath>

#include <memory>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class DBusMenuExporter;
class StatusNotifierItem;
class StatusNotifierItemDBus;
class StatusNotifierItemAdaptor;

class StatusNotifierItemDBusPrivate : public QObject
{
    Q_OBJECT

    friend class StatusNotifierItem;
    friend class StatusNotifierItemDBus;

public:
    StatusNotifierItemDBusPrivate(StatusNotifierItemDBus*);

    void init();
    void registerToHost();

    StatusNotifierItem*              sni;
    StatusNotifierItemDBus*          q;
    StatusNotifierItemAdaptor*       adaptor;
    QDBusObjectPath                  menuObjectPath;
    DBusMenuExporter*                menuExporter { nullptr };
    QMenu*                           menu { nullptr };
    std::unique_ptr<QDBusConnection> sessionBus;
    QString                          service;

    static int                       serviceCounter;

public Q_SLOTS:
    void onMenuDestroyed();
    void onServiceOwnerChanged(
        const QString &service, const QString &oldOwner, const QString &newOwner
    );
};
