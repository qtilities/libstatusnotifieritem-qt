/*
    SPDX-FileCopyrightText:  2015 LXQt team, 2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
    Authors:
        Balázs Béla     <balazsbela[at]gmail.com>
        Paulo Lieuthier <paulolieuthier@gmail.com>
*/
#include <QDBusArgument>

#ifndef DBUSTYPES_H
#define DBUSTYPES_H

/*!
    ARGB32 binary representation of the icon.
*/
struct IconPixmap {
    int width;        //!< The icon width. @todo pixels?
    int height;       //!< The icon height.
    QByteArray bytes; //!< The icon data.
};

/*!
    All the icons can be transferred over the bus by a particular serialization
    of their data, capable of representing multiple resolutions of the same image
    or a brief animation of images of the same size.

    Icons are transferred in an array of raw image data structures of signature
    a(iiay) whith each one describing the width, height, and image data respectively.
    The data is represented in ARGB32 format and is in the network byte order,
    to make easy the communication over the network between little and big endian machines.
*/
typedef QList<IconPixmap> IconPixmapList;

Q_DECLARE_METATYPE(IconPixmap)
Q_DECLARE_METATYPE(IconPixmapList)

/*!
    Data structure that describes extra information associated to this item,
    that can be visualized for instance by a tooltip
    (or by any other mean the visualization consider appropriate).
*/
struct ToolTip {
    //! Freedesktop-compliant name for an icon.
    QString iconName;
    //! Icon data.
    QList<IconPixmap> iconPixmap;
    //! Title for this tooltip.
    QString title;
    //! @brief Descriptive text for this tooltip.
    //! It can contain also a subset of the HTML markup language,
    //! for a list of allowed tags see Section [Markup].
    //! [Markup]: https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/Markup/
    QString description;
};

Q_DECLARE_METATYPE(ToolTip)

QDBusArgument &operator<<(QDBusArgument &argument, const IconPixmap &icon);
const QDBusArgument &operator>>(const QDBusArgument &argument, IconPixmap &icon);

QDBusArgument &operator<<(QDBusArgument &argument, const ToolTip &toolTip);
const QDBusArgument &operator>>(const QDBusArgument &argument, ToolTip &toolTip);

#endif // DBUSTYPES_H
