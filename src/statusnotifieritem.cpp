/*
    SPDX-FileCopyrightText:  2015 LXQt team, 2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
    Authors:
        Paulo Lieuthier <paulolieuthier@gmail.com>
*/
#include "statusnotifieritem.h"
#include "statusnotifieritem_p.h"
#include "statusnotifieritemadaptor.h"

#include <dbusmenuexporter.h>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusServiceWatcher>

#include <QIcon>
#include <QMenu>

#include <utility>

StatusNotifierItem::StatusNotifierItem(QString id, QObject *parent)
    : QObject(parent)
    , d(new StatusNotifierItemPrivate(this))
{
    d->init(id);
}

StatusNotifierItem::~StatusNotifierItem()
{
    d->sessionBus->unregisterObject(QLatin1String("/StatusNotifierItem"));
    QDBusConnection::disconnectFromBus(d->service);
}

QString StatusNotifierItem::id() const
{
    return d->id;
}

QString StatusNotifierItem::title() const
{
    return d->title;
}

void StatusNotifierItem::setTitle(const QString &title)
{
    if (d->title == title)
        return;

    d->title = title;
    Q_EMIT d->adaptor->NewTitle();
}

StatusNotifierItem::ItemStatus StatusNotifierItem::status() const
{
    return d->status;
}

void StatusNotifierItem::setStatus(ItemStatus status)
{
    if (d->status == status)
        return;

    d->status = status;

#ifdef QT_DBUS_LIB
    Q_EMIT d->adaptor->NewStatus(
        QString::fromLatin1(metaObject()->enumerator(metaObject()->indexOfEnumerator("ItemStatus")).valueToKey(d->status))
    );
#endif
}

StatusNotifierItem::ItemCategory StatusNotifierItem::category() const
{
    return d->category;
}

void StatusNotifierItem::setCategory(ItemCategory category)
{
    if (d->category == category)
        return;

    d->category = category;
}

void StatusNotifierItem::setMenuPath(const QString& path)
{
    d->menuObjectPath.setPath(path);
}

QDBusObjectPath StatusNotifierItem::menu() const
{
    return d->menuObjectPath;
}

void StatusNotifierItem::setIconByName(const QString &name)
{
    if (d->iconName == name)
        return;

    d->iconName = name;
    Q_EMIT d->adaptor->NewIcon();
}

QString StatusNotifierItem::iconName() const
{
    return d->iconName;
}

IconPixmapList StatusNotifierItem::iconPixmap() const
{
    return d->icon;
}

void StatusNotifierItem::setIconByPixmap(const QIcon &icon)
{
    if (d->iconCacheKey == icon.cacheKey())
        return;

    d->iconCacheKey = icon.cacheKey();
    d->icon = d->iconToPixmapList(icon);
    d->iconName.clear();
    Q_EMIT d->adaptor->NewIcon();
}

QString StatusNotifierItem::overlayIconName() const
{
    return d->overlayIconName;
}

void StatusNotifierItem::setOverlayIconByName(const QString &name)
{
    if (d->overlayIconName == name)
        return;

    d->overlayIconName = name;
    Q_EMIT d->adaptor->NewOverlayIcon();
}

IconPixmapList StatusNotifierItem::overlayIconPixmap() const
{
    return d->overlayIcon;
}

void StatusNotifierItem::setOverlayIconByPixmap(const QIcon &icon)
{
    if (d->overlayIconCacheKey == icon.cacheKey())
        return;

    d->overlayIconCacheKey = icon.cacheKey();
    d->overlayIcon = d->iconToPixmapList(icon);
    d->overlayIconName.clear();
    Q_EMIT d->adaptor->NewOverlayIcon();
}

QString StatusNotifierItem::attentionIconName() const
{
    return d->attentionIconName;
}

void StatusNotifierItem::setAttentionIconByName(const QString &name)
{
    if (d->attentionIconName == name)
        return;

    d->attentionIconName = name;
    Q_EMIT d->adaptor->NewAttentionIcon();
}

IconPixmapList StatusNotifierItem::attentionIconPixmap() const
{
    return d->attentionIcon;
}

void StatusNotifierItem::setAttentionIconByPixmap(const QIcon &icon)
{
    if (d->attentionIconCacheKey == icon.cacheKey())
        return;

    d->attentionIconCacheKey = icon.cacheKey();
    d->attentionIcon = d->iconToPixmapList(icon);
    d->attentionIconName.clear();
    Q_EMIT d->adaptor->NewAttentionIcon();
}

QString StatusNotifierItem::toolTipTitle() const
{
    return d->tooltipTitle;
}

void StatusNotifierItem::setToolTipTitle(const QString &title)
{
    if (d->tooltipTitle == title)
        return;

    d->tooltipTitle = title;
    Q_EMIT d->adaptor->NewToolTip();
}

QString StatusNotifierItem::toolTipSubTitle() const
{
    return d->tooltipSubtitle;
}

void StatusNotifierItem::setToolTipSubTitle(const QString &subTitle)
{
    if (d->tooltipSubtitle == subTitle)
        return;

    d->tooltipSubtitle = subTitle;
    Q_EMIT d->adaptor->NewToolTip();
}

QString StatusNotifierItem::toolTipIconName() const
{
    return d->tooltipIconName;
}

void StatusNotifierItem::setToolTipIconByName(const QString &name)
{
    if (d->tooltipIconName == name)
        return;

    d->tooltipIconName = name;
    Q_EMIT d->adaptor->NewToolTip();
}

IconPixmapList StatusNotifierItem::toolTipIconPixmap() const
{
    return d->tooltipIcon;
}

void StatusNotifierItem::setToolTipIconByPixmap(const QIcon &icon)
{
    if (d->tooltipIconCacheKey == icon.cacheKey())
        return;

    d->tooltipIconCacheKey = icon.cacheKey();
    d->tooltipIcon = d->iconToPixmapList(icon);
    d->tooltipIconName.clear();
    Q_EMIT d->adaptor->NewToolTip();
}

ToolTip StatusNotifierItem::toolTip() const
{
    ToolTip tt;
    tt.title       = d->tooltipTitle;
    tt.description = d->tooltipSubtitle;
    tt.iconName    = d->tooltipIconName;
    tt.iconPixmap  = d->tooltipIcon;
    return tt;
}

void StatusNotifierItem::setContextMenu(QMenu* menu)
{
    if (d->menu == menu)
        return;

    if (d->menu)
        QObject::disconnect(d->menu, &QObject::destroyed, d.get(), &StatusNotifierItemPrivate::onMenuDestroyed);

    d->menu = menu;

    if (d->menu)
        setMenuPath(QLatin1String("/MenuBar"));
    else
        setMenuPath(QLatin1String("/NO_DBUSMENU"));

    // Note: we need to destroy menu exporter before creating new one
    // to free the DBus object path for new menu
    delete d->menuExporter;

    if (d->menu) {
        QObject::connect(d->menu, &QObject::destroyed, d.get(), &StatusNotifierItemPrivate::onMenuDestroyed);
        d->menuExporter = new DBusMenuExporter{this->menu().path(), d->menu, *d->sessionBus.get()};
    }
}

QMenu* StatusNotifierItem::contextMenu() const
{
    return d->menu;
}

void StatusNotifierItem::Activate(int x, int y)
{
    if (d->status == StatusNotifierItem::NeedsAttention)
        d->status = StatusNotifierItem::Active;

    Q_EMIT activateRequested(QPoint(x, y));
}

void StatusNotifierItem::SecondaryActivate(int x, int y)
{
    if (d->status == StatusNotifierItem::NeedsAttention)
        d->status = StatusNotifierItem::Active;

    Q_EMIT secondaryActivateRequested(QPoint(x, y));
}

void StatusNotifierItem::ContextMenu(int x, int y)
{
    if (d->menu != nullptr)
    {
        if (d->menu->isVisible())
            d->menu->popup(QPoint(x, y));
        else
            d->menu->hide();
    }
}

void StatusNotifierItem::Scroll(int delta, const QString &orientation)
{
    Qt::Orientation orient = Qt::Vertical;
    if (orientation.toLower() == QLatin1String("horizontal"))
        orient = Qt::Horizontal;

    Q_EMIT scrollRequested(delta, orient);
}
//==============================================================================
// StatusNotifierItemPrivate
//==============================================================================
int StatusNotifierItemPrivate::serviceCounter = 0;

StatusNotifierItemPrivate::StatusNotifierItemPrivate(StatusNotifierItem* item)
    : q(item)
    , category(StatusNotifierItem::ApplicationStatus)
    , status(StatusNotifierItem::Active)
{
}

void StatusNotifierItemPrivate::init(QString extraId)
{
    id              = std::move(extraId);
    title           = QLatin1String("Test");
#ifdef QT_DBUS_LIB
    adaptor         = new StatusNotifierItemAdaptor(q);
    service         = QString::fromLatin1("org.freedesktop.StatusNotifierItem-%1-%2")
                        .arg(QCoreApplication::applicationPid(), ++serviceCounter);
    sessionBus      = std::make_unique<QDBusConnection>(QDBusConnection::connectToBus(QDBusConnection::SessionBus, service));

    menuObjectPath.setPath(QLatin1String("/NO_DBUSMENU"));

    // Register DBus meta types
    qDBusRegisterMetaType<IconPixmap>();
    qDBusRegisterMetaType<IconPixmapList>();
    qDBusRegisterMetaType<ToolTip>();

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
        this, &StatusNotifierItemPrivate::onServiceOwnerChanged
    );
#endif
}

void StatusNotifierItemPrivate::registerToHost()
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

void StatusNotifierItemPrivate::onMenuDestroyed()
{
    menu = nullptr;
    q->setMenuPath(QLatin1String("/NO_DBUSMENU"));
    // menu is a QObject parent of the menuExporter
    menuExporter = nullptr;
}

void StatusNotifierItemPrivate::onServiceOwnerChanged(
    const QString& service,
    const QString& oldOwner,
    const QString& newOwner
) {
    Q_UNUSED(service)
    Q_UNUSED(oldOwner)

    if (!newOwner.isEmpty())
        registerToHost();
}

IconPixmapList StatusNotifierItemPrivate::iconToPixmapList(const QIcon& icon)
{
    IconPixmapList pixmapList;

    // long live KDE!
    const QList<QSize> sizes = icon.availableSizes();
    for (const QSize &size : sizes) {
        QImage image = icon.pixmap(size).toImage();

        IconPixmap pix;
        pix.height = image.height();
        pix.width = image.width();

        if (image.format() != QImage::Format_ARGB32)
            image = image.convertToFormat(QImage::Format_ARGB32);

        pix.bytes = QByteArray((char *) image.bits(), image.sizeInBytes());

        // swap to network byte order if we are little endian
        if (QSysInfo::ByteOrder == QSysInfo::LittleEndian) {
            quint32 *uintBuf = (quint32 *) pix.bytes.data();

            for (uint i = 0; i < pix.bytes.size() / sizeof(quint32); ++i) {
                *uintBuf = qToBigEndian(*uintBuf);
                ++uintBuf;
            }
        }
        pixmapList.append(pix);
    }
    return pixmapList;
}
