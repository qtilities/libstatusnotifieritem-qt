/*
    SPDX-FileCopyrightText:  2015 LXQt team, 2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
    Authors:
        Paulo Lieuthier <paulolieuthier@gmail.com>
*/
#include "statusnotifieritem.h"
#include "statusnotifieritem_p.h"
#include "statusnotifieritemdbus_p.hpp"
#include "statusnotifieritemdbus_p_p.hpp"
#include "statusnotifieritemadaptor.h"

#include <QtEndian>
#include <QIcon>
#include <QMenu>

#include <utility>

StatusNotifierItem::StatusNotifierItem(QString id, QObject* parent)
    : QObject(parent)
    , d(new StatusNotifierItemPrivate(this))
{
    d->init(id);
}

StatusNotifierItem::~StatusNotifierItem()
{
}

QString StatusNotifierItem::id() const
{
    return d->id;
}

void StatusNotifierItem::setCategory(SNICategory category)
{
    if (d->category == category)
        return;

    d->category = category;
}

StatusNotifierItem::SNICategory StatusNotifierItem::category() const
{
    return d->category;
}

void StatusNotifierItem::setStatus(SNIStatus status)
{
    if (d->status == status)
        return;

    d->status = status;

#ifdef QT_DBUS_LIB
    Q_EMIT d->dbus->d->adaptor->NewStatus(QString::fromLatin1(
        metaObject()->enumerator(metaObject()->indexOfEnumerator("SNIStatus")).valueToKey(d->status))
    );
#endif
}

StatusNotifierItem::SNIStatus StatusNotifierItem::status() const
{
    return d->status;
}

void StatusNotifierItem::setTitle(const QString &title)
{
    if (d->title == title)
        return;

    d->title = title;

#ifdef QT_DBUS_LIB
    Q_EMIT d->dbus->d->adaptor->NewTitle();
#endif
}

QString StatusNotifierItem::title() const
{
    return d->title;
}

void StatusNotifierItem::setIconByName(const QString &name)
{
    if (d->iconName == name)
        return;

    d->iconName = name;

#ifdef QT_DBUS_LIB
    d->serializedIcon = SNIIconList();
    Q_EMIT d->dbus->d->adaptor->NewIcon();
#endif
}

QString StatusNotifierItem::iconName() const
{
    return d->iconName;
}

void StatusNotifierItem::setIconByPixmap(const QIcon &icon)
{
    if (d->iconName.isEmpty() && d->iconCacheKey == icon.cacheKey())
        return;

    d->iconCacheKey = icon.cacheKey();
    d->iconName.clear();
    d->icon = icon;

#ifdef QT_DBUS_LIB
    d->serializedIcon = d->iconToPixmapList(icon);
    Q_EMIT d->dbus->d->adaptor->NewIcon();
#endif
}

QIcon StatusNotifierItem::iconPixmap() const
{
    return d->icon;
}

void StatusNotifierItem::setOverlayIconByName(const QString &name)
{
    if (d->overlayIconName == name)
        return;

    d->overlayIconName = name;

#ifdef QT_DBUS_LIB
    Q_EMIT d->dbus->d->adaptor->NewOverlayIcon();
#endif
}

QString StatusNotifierItem::overlayIconName() const
{
    return d->overlayIconName;
}

void StatusNotifierItem::setOverlayIconByPixmap(const QIcon &icon)
{
    if (d->overlayIconName.isEmpty() && d->overlayIconCacheKey == icon.cacheKey())
        return;

    d->overlayIconCacheKey = icon.cacheKey();
    d->overlayIconName.clear();
    d->overlayIcon = icon;

#ifdef QT_DBUS_LIB
    d->serializedOverlayIcon = d->iconToPixmapList(icon);
    Q_EMIT d->dbus->d->adaptor->NewOverlayIcon();
#endif
}

QIcon StatusNotifierItem::overlayIconPixmap() const
{
    return d->overlayIcon;
}

void StatusNotifierItem::setAttentionIconByName(const QString &name)
{
    if (d->attentionIconName == name)
        return;

    d->attentionIconName = name;

#ifdef QT_DBUS_LIB
    d->serializedAttentionIcon = SNIIconList();
    Q_EMIT d->dbus->d->adaptor->NewAttentionIcon();
#endif
}

QString StatusNotifierItem::attentionIconName() const
{
    return d->attentionIconName;
}

void StatusNotifierItem::setAttentionIconByPixmap(const QIcon &icon)
{
    if (d->attentionIconName.isEmpty() && d->attentionIconCacheKey == icon.cacheKey())
        return;

    d->attentionIconCacheKey = icon.cacheKey();
    d->attentionIconName.clear();
    d->attentionIcon = icon;

#ifdef QT_DBUS_LIB
    d->serializedAttentionIcon = d->iconToPixmapList(icon);
    Q_EMIT d->dbus->d->adaptor->NewAttentionIcon();
#endif
}

QIcon StatusNotifierItem::attentionIconPixmap() const
{
    return d->attentionIcon;
}

void StatusNotifierItem::setToolTip(const QString &iconName, const QString &title, const QString &subTitle)
{
    if (d->toolTipIconName == iconName && d->toolTipTitle == title && d->toolTipSubTitle == subTitle)
        return;

    d->toolTipIconName = iconName;
    d->toolTipTitle    = title;
    d->toolTipSubTitle = subTitle;

#ifdef QT_DBUS_LIB
    d->serializedToolTipIcon = SNIIconList();
    Q_EMIT d->dbus->d->adaptor->NewToolTip();
#endif
}

void StatusNotifierItem::setToolTip(const QIcon& icon, const QString& title, const QString& subTitle)
{
    if (d->toolTipIconName.isEmpty() &&
        d->toolTipIcon.cacheKey() == icon.cacheKey() &&
        d->toolTipTitle           == title &&
        d->toolTipSubTitle        == subTitle) {
        return;
    }
    d->toolTipIconName.clear();

    d->toolTipIcon     = icon;
    d->toolTipTitle    = title;
    d->toolTipSubTitle = subTitle;

#ifdef QT_DBUS_LIB
    d->serializedToolTipIcon = d->iconToPixmapList(icon);
    Q_EMIT d->dbus->d->adaptor->NewToolTip();
#endif
}

void StatusNotifierItem::setToolTipIconByName(const QString &name)
{
    if (d->toolTipIconName == name)
        return;

    d->toolTipIconName = name;

#ifdef QT_DBUS_LIB
    d->serializedToolTipIcon = SNIIconList();
    Q_EMIT d->dbus->d->adaptor->NewToolTip();
#endif
}

QString StatusNotifierItem::toolTipIconName() const
{
    return d->toolTipIconName;
}

void StatusNotifierItem::setToolTipIconByPixmap(const QIcon &icon)
{
    if (d->toolTipIconName.isEmpty() && d->toolTipIconCacheKey == icon.cacheKey())
        return;

    d->toolTipIconCacheKey = icon.cacheKey();
    d->toolTipIconName.clear();
    d->toolTipIcon = icon;

#ifdef QT_DBUS_LIB
    d->serializedToolTipIcon = d->iconToPixmapList(icon);
    Q_EMIT d->dbus->d->adaptor->NewToolTip();
#endif
}

QIcon StatusNotifierItem::toolTipIconPixmap() const
{
    return d->toolTipIcon;
}

void StatusNotifierItem::setToolTipTitle(const QString &title)
{
    if (d->toolTipTitle == title)
        return;

    d->toolTipTitle = title;

#ifdef QT_DBUS_LIB
    Q_EMIT d->dbus->d->adaptor->NewToolTip();
#endif
}

QString StatusNotifierItem::toolTipTitle() const
{
    return d->toolTipTitle;
}

void StatusNotifierItem::setToolTipSubTitle(const QString &subTitle)
{
    if (d->toolTipSubTitle == subTitle)
        return;

    d->toolTipSubTitle = subTitle;

#ifdef QT_DBUS_LIB
    Q_EMIT d->dbus->d->adaptor->NewToolTip();
#endif
}

QString StatusNotifierItem::toolTipSubTitle() const
{
    return d->toolTipSubTitle;
}

void StatusNotifierItem::setContextMenu(QMenu* menu)
{
#ifdef QT_DBUS_LIB
    d->dbus->setContextMenu(menu);
#endif
}

QMenu* StatusNotifierItem::contextMenu() const
{
#ifdef QT_DBUS_LIB
    return d->dbus->contextMenu();
#endif
}
//==============================================================================
// StatusNotifierItemPrivate
//==============================================================================
StatusNotifierItemPrivate::StatusNotifierItemPrivate(StatusNotifierItem* sni)
    : q(sni)
    , category(StatusNotifierItem::ApplicationStatus)
    , status(StatusNotifierItem::Active)
{
}

void StatusNotifierItemPrivate::init(QString extraId)
{
    id    = std::move(extraId);
    title = QLatin1String("Test");
#ifdef QT_DBUS_LIB
    dbus  = new StatusNotifierItemDBus(q);
#endif
}

#ifdef QT_DBUS_LIB
SNIIconList StatusNotifierItemPrivate::iconToPixmapList(const QIcon& icon)
{
    SNIIconList pixmapList;

    const QList<QSize> sizes = icon.availableSizes();
    for (const QSize &size : sizes) {
        QImage image = icon.pixmap(size).toImage();

        SNIIcon pix;
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
#endif
