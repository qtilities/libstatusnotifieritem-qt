/*
    SPDX-FileCopyrightText:  2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
*/
#pragma once

#include <QDBusArgument>
#include <QDBusObjectPath>
#include <QObject>
#include <QString>

#include <memory>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE
//==================================================================================================
// DBus types
//==================================================================================================
/*!
    ARGB32 binary representation of the icon.
*/
struct SNIIcon {
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
typedef QList<SNIIcon> SNIIconList;

/*!
    Data structure that describes extra information associated to this item,
    that can be visualized for instance by a tooltip
    (or by any other mean the visualization consider appropriate).
*/
struct SNIToolTip {
    //! Freedesktop-compliant name for an icon.
    QString iconName;
    //! Icon data.
    QList<SNIIcon> iconPixmap;
    //! Title for this tooltip.
    QString title;
    //! @brief Descriptive text for this tooltip.
    //! It can contain also a subset of the HTML markup language,
    //! for a list of allowed tags see Section [Markup].
    //! [Markup]: https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/Markup/
    QString description;
};

Q_DECLARE_METATYPE(SNIIcon)
Q_DECLARE_METATYPE(SNIIconList)
Q_DECLARE_METATYPE(SNIToolTip)

QDBusArgument &operator<<(QDBusArgument &argument, const SNIIcon &icon);
const QDBusArgument &operator>>(const QDBusArgument &argument, SNIIcon &icon);

QDBusArgument &operator<<(QDBusArgument &argument, const SNIToolTip &toolTip);
const QDBusArgument &operator>>(const QDBusArgument &argument, SNIToolTip &toolTip);

//==================================================================================================
// StatusNotifierItemDBus
//==================================================================================================
class StatusNotifierItem;
class StatusNotifierItemDBusPrivate;
class StatusNotifierItemAdaptor;

class StatusNotifierItemDBus : public QObject
{
    Q_OBJECT

    /*!
        Describes the category of this item.
        @ref StatusNotifierItem::SNICategory.
    */
    Q_PROPERTY(QString Category READ category)

    /*!
        It's a name that should be unique for this application
        and consistent between sessions, such as the application
        name itself (QCoreApplication::applicationName()).
    */
    Q_PROPERTY(QString Id READ id)

    /*!
        Sets the name that describes the application,
        it can be more descriptive than @ref Id.
        @see StatusNotifierItem::id() StatusNotifierItem()
    */
    Q_PROPERTY(QString Title READ title)

    /*!
        The status of this item or of the associated application, see @ref SNIStatus.
        @see title() setTitle()
    */
    Q_PROPERTY(QString Status READ status)

    /*!
        It's the windowing-system dependent identifier for a window.
        The application can chose one of its windows to be available
        through this property or just set 0 if it's not interested.
    */
    Q_PROPERTY(uint32_t WindowId READ windowId)

    /*!
        The StatusNotifierItem can carry an icon that can be used
        by the visualization to identify the item.

        An icon can either be identified by its Freedesktop-compliant icon name,
        carried by this property or by the icon data itself,
        carried by the property IconPixmap. Visualizations are encouraged
        to prefer icon names over icon pixmaps if both are available.

        @see iconName() setIconByName()
    */
    Q_PROPERTY(QString IconName READ iconName)

    /*!
        Carries an ARGB32 binary representation of the icon.
        the format of icon data used in this specification is described in
        [Section Icons](https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/Icons/)
    */
    Q_PROPERTY(SNIIconList IconPixmap READ iconPixmap)

    /*!
        The Freedesktop-compliant name of an icon.
        This can be used by the visualization to indicate extra state information,
        for instance as an overlay for the main icon.
    */
    Q_PROPERTY(QString OverlayIconName READ overlayIconName)

    /*!
        ARGB32 binary representation of the overlay icon described in the previous paragraph.
    */
    Q_PROPERTY(SNIIconList OverlayIconPixmap READ overlayIconPixmap)

    /*!
        The Freedesktop-compliant name of an icon. this can be used by the visualization
        to indicate that the item is in RequestingAttention state.
    */
    Q_PROPERTY(QString AttentionIconName READ attentionIconName)

    /*!
        ARGB32 binary representation of the requesting attention icon
        described in the previous paragraph.
    */
    Q_PROPERTY(SNIIconList AttentionIconPixmap READ attentionIconPixmap)
#if 0
    /*!
        @todo attentionMovieName() unimplemented.

        An item can also specify an animation associated to the RequestingAttention state.
        This should be either a Freedesktop-compliant icon name or a full path.
        The visualization can chose between the movie or AttentionIconPixmap
        (or using neither of those) at its discretion.

        @see attentionMovieName()
    */
    Q_PROPERTY(QString AttentionMovieName READ attentionMovieName)
#endif
    /*!
        Item tooltip, see @ref ToolTip.
        @see toolTip()
    */
    Q_PROPERTY(SNIToolTip ToolTip READ toolTip)

    /*!
        @todo: itemIsMenu() unimplemented.

        The item only support the context menu, the visualization should prefer
        showing the menu or sending ContextMenu() instead of Activate().
        @see itemIsMenu()
    */
    Q_PROPERTY(bool ItemIsMenu READ itemIsMenu)

    /*!
        DBus path to an object which should implement
        the [com.canonical.dbusmenu] interface.
        @see menu() setMenuPath()

        [com.canonical.dbusmenu]: https://github.com/qtilities/libdbusmenu-qtilities/
    */
    Q_PROPERTY(QDBusObjectPath Menu READ menuPath)

    friend class StatusNotifierItem;

public:
    explicit StatusNotifierItemDBus(StatusNotifierItem*);
    ~StatusNotifierItemDBus() override;

    QString     id() const;
    QString     category() const;
    QString     status() const;
    QString     title() const;
    QString     iconName() const;
    SNIIconList iconPixmap() const;
    QString     overlayIconName() const;
    SNIIconList overlayIconPixmap() const;
    QString     attentionIconName() const;
    SNIIconList attentionIconPixmap() const;
#if 0
    QString     attentionMovieName() const;
#endif
    uint32_t    windowId() const;

    /*!
        @return a SNIToolTip object instance.
    */
    SNIToolTip toolTip() const;

    /*!
        Sets the menu object path.
        @see Menu
    */
    void setMenuPath(const QString&);

    /*!
        @return the DBus menu path.
        @see Menu
    */
    QDBusObjectPath menuPath() const;

    /*!
        @todo: Unimplemented.
        @return if this item supports only a context menu.
        @see Menu
    */
    bool itemIsMenu() const;

    void setContextMenu(QMenu*);
    QMenu* contextMenu() const;

public Q_SLOTS:
    /*!
        Asks the status notifier item to show a context menu.

        This is typically a consequence of user input,
        such as mouse right click over the graphical representation of the item.

        The x and y parameters are in screen coordinates and is to be considered
        an hint to the item about where to show the context menu.

        @param x The X coordinate.
        @param y The Y coordinate.
    */
    void ContextMenu(int x, int y);

    /*!
        Asks the status notifier item for activation.

        This is typically a consequence of user input,
        such as mouse left click over the graphical representation of the item.
        The application will perform any task is considered appropriate as an activation request.

        The x and y parameters are in screen coordinates and is to be considered
        an hint to the item where to show eventual windows (if any).

        @param x The X coordinate.
        @param y The Y coordinate.
    */
    void Activate(int x, int y);

    /*!
        Secondary and less important form of activation compared to Activate.

        This is typically a consequence of user input,
        such as mouse middle click over the graphical representation of the item.
        The application will perform any task is considered appropriate as an activation request.

        The x and y parameters are in screen coordinates and is to be considered
        an hint to the item where to show eventual windows (if any).

        @param x The X coordinate.
        @param y The Y coordinate.
    */
    void SecondaryActivate(int x, int y);

    /*!
        The user asked for a scroll action.

        This is typically a consequence of user input,
        such as mouse wheel over the graphical representation of the item.

        @param delta       Represents the amount of scroll.
        @param orientation Represents the horizontal or vertical orientation.
    */
    void Scroll(int delta, const QString &orientation);

// Q_SIGNALS are in StatusNotifierItemAdaptor

private:
    std::unique_ptr<StatusNotifierItemDBusPrivate> const d;
};
