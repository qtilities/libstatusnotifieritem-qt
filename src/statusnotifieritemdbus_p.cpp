/*
    SPDX-FileCopyrightText:  2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
*/
#include "statusnotifieritemdbus_p.hpp"
#include "statusnotifieritemdbus_p_p.hpp"
#include "statusnotifieritem.h"
#include "statusnotifieritem_p.h"
#include "statusnotifieritemadaptor.h"

#include <dbusmenuexporter.h>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QMenu>
//==================================================================================================
// DBus types
//==================================================================================================

// Marshall the SNIIcon data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const SNIIcon& icon)
{
    argument.beginStructure();
    argument << icon.width;
    argument << icon.height;
    argument << icon.bytes;
    argument.endStructure();
    return argument;
}

// Retrieve the ImageStruct data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, SNIIcon& icon)
{
    argument.beginStructure();
    argument >> icon.width;
    argument >> icon.height;
    argument >> icon.bytes;
    argument.endStructure();
    return argument;
}

// Marshall the ToolTip data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const SNIToolTip &toolTip)
{
    argument.beginStructure();
    argument << toolTip.iconName;
    argument << toolTip.iconPixmap;
    argument << toolTip.title;
    argument << toolTip.description;
    argument.endStructure();
    return argument;
}

// Retrieve the ToolTip data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, SNIToolTip &toolTip)
{
    argument.beginStructure();
    argument >> toolTip.iconName;
    argument >> toolTip.iconPixmap;
    argument >> toolTip.title;
    argument >> toolTip.description;
    argument.endStructure();
    return argument;
}
//==================================================================================================
// StatusNotifierItemDBus
//==================================================================================================

StatusNotifierItemDBus::StatusNotifierItemDBus(StatusNotifierItem* parent)
    : QObject(parent)
    , d(new StatusNotifierItemDBusPrivate(this))
{
    d->init();
}

StatusNotifierItemDBus::~StatusNotifierItemDBus()
{
    d->sessionBus->unregisterObject(QLatin1String("/StatusNotifierItem"));
    QDBusConnection::disconnectFromBus(d->service);
}

QString StatusNotifierItemDBus::id() const
{
    return d->sni->d->id;
}

QString StatusNotifierItemDBus::category() const
{
    return QLatin1String(
        d->sni->metaObject()->enumerator(
        d->sni->metaObject()->indexOfEnumerator("SNICategory")).valueToKey(d->sni->category()));
}

QString StatusNotifierItemDBus::status() const
{
    return QLatin1String(
        d->sni->metaObject()->enumerator(
        d->sni->metaObject()->indexOfEnumerator("SNIStatus")).valueToKey(d->sni->status()));
}

QString StatusNotifierItemDBus::title() const
{
    return d->sni->d->title;
}

QString StatusNotifierItemDBus::iconName() const
{
    return d->sni->d->iconName;
}

SNIIconList StatusNotifierItemDBus::iconPixmap() const
{
    return d->sni->d->serializedIcon;
}

QString StatusNotifierItemDBus::overlayIconName() const
{
    return d->sni->d->overlayIconName;
}

SNIIconList StatusNotifierItemDBus::overlayIconPixmap() const
{
    return d->sni->d->serializedOverlayIcon;
}

QString StatusNotifierItemDBus::attentionIconName() const
{
    return d->sni->d->attentionIconName;
}

SNIIconList StatusNotifierItemDBus::attentionIconPixmap() const
{
    return d->sni->d->serializedAttentionIcon;
}

uint32_t StatusNotifierItemDBus::windowId() const
{
    return 0;
}

SNIToolTip StatusNotifierItemDBus::toolTip() const
{
    SNIToolTip tt;
    tt.iconName    = d->sni->d->toolTipIconName;
    tt.iconPixmap  = d->sni->d->serializedToolTipIcon;
    tt.title       = d->sni->d->toolTipTitle;
    tt.description = d->sni->d->toolTipSubTitle;
    return tt;
}

bool StatusNotifierItemDBus::itemIsMenu() const
{
    return false;
}

void StatusNotifierItemDBus::setMenuPath(const QString& path)
{
    d->menuObjectPath.setPath(path);
}

QDBusObjectPath StatusNotifierItemDBus::menuPath() const
{
    return d->menuObjectPath;
}

void StatusNotifierItemDBus::setContextMenu(QMenu* menu)
{
    if (d->menu == menu)
        return;

    if (d->menu)
        QObject::disconnect(d->menu, &QObject::destroyed, d.get(), &StatusNotifierItemDBusPrivate::onMenuDestroyed);

    d->menu = menu;

    if (d->menu)
        setMenuPath(QLatin1String("/MenuBar"));
    else
        setMenuPath(QLatin1String("/NO_DBUSMENU"));

    // Note: we need to destroy menu exporter before creating new one
    // to free the DBus object path for new menu
    delete d->menuExporter;

    if (d->menu) {
        QObject::connect(d->menu, &QObject::destroyed, d.get(), &StatusNotifierItemDBusPrivate::onMenuDestroyed);
        d->menuExporter = new DBusMenuExporter{d->menuObjectPath.path(), d->menu, *d->sessionBus.get()};
    }
}

QMenu* StatusNotifierItemDBus::contextMenu() const
{
    return d->menu;
}

void StatusNotifierItemDBus::Activate(int x, int y)
{
    if (d->sni->status() == StatusNotifierItem::NeedsAttention)
        d->sni->setStatus(StatusNotifierItem::Active);

    Q_EMIT d->sni->activateRequested(QPoint(x, y));
}

void StatusNotifierItemDBus::SecondaryActivate(int x, int y)
{
    if (d->sni->status() == StatusNotifierItem::NeedsAttention)
        d->sni->setStatus(StatusNotifierItem::Active);

    Q_EMIT d->sni->secondaryActivateRequested(QPoint(x, y));
}

void StatusNotifierItemDBus::ContextMenu(int x, int y)
{
    if (d->menu != nullptr)
    {
        if (d->menu->isVisible())
            d->menu->popup(QPoint(x, y));
        else
            d->menu->hide();
    }
}

void StatusNotifierItemDBus::Scroll(int delta, const QString &orientation)
{
    Qt::Orientation orient = Qt::Vertical;
    if (orientation.toLower() == QLatin1String("horizontal"))
        orient = Qt::Horizontal;

    Q_EMIT d->sni->scrollRequested(delta, orient);
}
//==================================================================================================
// StatusNotifierItemDBusPrivate
//==================================================================================================
int StatusNotifierItemDBusPrivate::serviceCounter = 0;

StatusNotifierItemDBusPrivate::StatusNotifierItemDBusPrivate(StatusNotifierItemDBus* owner)
    : q(owner)
{
}

void StatusNotifierItemDBusPrivate::init()
{
    sni     = static_cast<StatusNotifierItem*>(q->parent());
    adaptor = new StatusNotifierItemAdaptor(q);
    service = QString::fromLatin1("org.freedesktop.StatusNotifierItem-%1-%2")
                        .arg(QCoreApplication::applicationPid(), ++serviceCounter);
    sessionBus =
        std::make_unique<QDBusConnection>(
            QDBusConnection::connectToBus(QDBusConnection::SessionBus, service)
        );

    menuObjectPath.setPath(QLatin1String("/NO_DBUSMENU"));

    // Register DBus meta types
    qDBusRegisterMetaType<SNIIcon>();
    qDBusRegisterMetaType<SNIIconList>();
    qDBusRegisterMetaType<SNIToolTip>();

    // Separate DBus connection to the session bus is created, because QDbus does not provide
    // a way to register different objects for different services with the same paths.
    // For status notifiers we need different /StatusNotifierItem for each service.

    // register service
    sessionBus->registerObject(QLatin1String("/StatusNotifierItem"), q);
    registerToHost();

    // monitor the watcher service in case the host restarts
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher(
        QLatin1String("org.kde.StatusNotifierWatcher"),
        *sessionBus.get(),
        QDBusServiceWatcher::WatchForOwnerChange,
        q
    );
    QObject::connect(
        watcher, &QDBusServiceWatcher::serviceOwnerChanged,
        this, &StatusNotifierItemDBusPrivate::onServiceOwnerChanged
    );
}

void StatusNotifierItemDBusPrivate::registerToHost()
{
    QDBusInterface interface(
        QLatin1String("org.kde.StatusNotifierWatcher"),
        QLatin1String("/StatusNotifierWatcher"),
        QLatin1String("org.kde.StatusNotifierWatcher"), *sessionBus.get()
    );
    interface.asyncCall(
        QLatin1String("RegisterStatusNotifierItem"),
        sessionBus->baseService()
    );
}

void StatusNotifierItemDBusPrivate::onMenuDestroyed()
{
    menu = nullptr;
    q->setMenuPath(QLatin1String("/NO_DBUSMENU"));
    // menu is a QObject parent of the menuExporter
    menuExporter = nullptr;
}

void StatusNotifierItemDBusPrivate::onServiceOwnerChanged(
    const QString& service,
    const QString& oldOwner,
    const QString& newOwner
) {
    Q_UNUSED(service)
    Q_UNUSED(oldOwner)

    if (!newOwner.isEmpty())
        registerToHost();
}
