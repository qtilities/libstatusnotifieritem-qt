/*
    SPDX-FileCopyrightText:  2015 LXQt team, 2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
    Authors:
        Balázs Béla     <balazsbela[at]gmail.com>
        Paulo Lieuthier <paulolieuthier@gmail.com>
*/
#include "dbustypes.h"

// Marshall the IconPixmap data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const IconPixmap &icon)
{
    argument.beginStructure();
    argument << icon.width;
    argument << icon.height;
    argument << icon.bytes;
    argument.endStructure();
    return argument;
}

// Retrieve the ImageStruct data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, IconPixmap &icon)
{
    argument.beginStructure();
    argument >> icon.width;
    argument >> icon.height;
    argument >> icon.bytes;
    argument.endStructure();
    return argument;
}

// Marshall the ToolTip data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const ToolTip &toolTip)
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
const QDBusArgument &operator>>(const QDBusArgument &argument, ToolTip &toolTip)
{
    argument.beginStructure();
    argument >> toolTip.iconName;
    argument >> toolTip.iconPixmap;
    argument >> toolTip.title;
    argument >> toolTip.description;
    argument.endStructure();
    return argument;
}
