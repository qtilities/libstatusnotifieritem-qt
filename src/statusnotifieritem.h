/*
    SPDX-FileCopyrightText:  2015 LXQt team, 2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
    Authors:
        Paulo Lieuthier <paulolieuthier@gmail.com>
*/
#ifndef STATUS_NOTIFIER_ITEM_H
#define STATUS_NOTIFIER_ITEM_H

#include <QObject>
#include <QIcon>
#include <QMenu>
#include <QDBusConnection>

#include "dbustypes.h"
#include "statusnotifieritem_export.h"

class StatusNotifierItemAdaptor;
class DBusMenuExporter;
/*!
    Qt implementation of the Freedesktop' [StatusNotifierItem] specification.

    Each application can register an arbitrary number of Status Notifier Items
    by registering on the session bus the service
    org.freedesktop.StatusNotifierItem-PID-ID, where PID is the process id
    of the application and ID is an arbitrary numeric unique identifier between
    different instances registered by the same application.

    As soon as a new instance of a StatusNotifierItem is created, the application
    must register the unique instance name to the StatusNotifierWatcher.

    Each instance of StatusNotifierItem must provide an object called
    StatusNotifierItem with the following properties, methods and signals.

    [StatusNotifierItem]: https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/
*/
class SNI_QT_EXPORT StatusNotifierItem : public QObject
{
    Q_OBJECT
    /*!
        Item category, see @ref ItemCategory.
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
        @see id() StatusNotifierItem()
    */
    Q_PROPERTY(QString Title READ title)
    /*!
        The status of this item or of the associated application, see @ref ItemStatus.
        @see title() setTitle()
    */
    Q_PROPERTY(QString Status READ status)
#if 0
    /*!
        It's the windowing-system dependent identifier for a window.
        The application can chose one of its windows to be available
        through this property or just set 0 if it's not interested.
    */
    Q_PROPERTY(uint32_t WindowId READ windowId)
#endif
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
    Q_PROPERTY(IconPixmapList IconPixmap READ iconPixmap)
    /*!
        The Freedesktop-compliant name of an icon.
        This can be used by the visualization to indicate extra state information,
        for instance as an overlay for the main icon.
    */
    Q_PROPERTY(QString OverlayIconName READ overlayIconName)
    /*!
        ARGB32 binary representation of the overlay icon described in the previous paragraph.
    */
    Q_PROPERTY(IconPixmapList OverlayIconPixmap READ overlayIconPixmap)
    /*!
        The Freedesktop-compliant name of an icon. this can be used by the visualization
        to indicate that the item is in RequestingAttention state.
    */
    Q_PROPERTY(QString AttentionIconName READ attentionIconName)
    /*!
        ARGB32 binary representation of the requesting attention icon
        described in the previous paragraph.
    */
    Q_PROPERTY(IconPixmapList AttentionIconPixmap READ attentionIconPixmap)
#if 0
    /*!
        TODO:
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
    Q_PROPERTY(ToolTip ToolTip READ toolTip)
#if 0
    /*!
        TODO:
        The item only support the context menu, the visualization should prefer
        showing the menu or sending ContextMenu() instead of Activate().
        @see itemIsMenu()
    */
    Q_PROPERTY(bool ItemIsMenu READ itemIsMenu)
#endif
    /*!
        DBus path to an object which should implement
        the [com.canonical.dbusmenu] interface.
        @see menu() setMenuPath()

        [com.canonical.dbusmenu]: https://github.com/qtilities/libdbusmenu-qtilities/
    */
    Q_PROPERTY(QDBusObjectPath Menu READ menu)

public:
    //! Describes the status of this item or of the associated application.
    //! The allowed values for the Status property are:
    enum ItemStatus {
        //! The item doesn't convey important information to the user,
        //! it can be considered an "idle" status and is likely
        //! that visualizations will chose to hide it.
        Passive = 1,
        //! The item is active, is more important that the item will be shown
        //! in some way to the user.
        Active = 2,
        //! The item carries really important information for the user,
        //! such as battery charge running out and is wants to incentive
        //! the direct user intervention.
        //! Visualizations should emphasize in some way the items with this status.
        NeedsAttention = 3,
    };
    Q_ENUM(ItemStatus)

    //! Describes the category of this item.
    //! The allowed values for the Category property are:
    enum ItemCategory {
        //! The item describes the status of a generic application,
        //! for instance the current state of a media player.
        //! In the case where the category of the item can not be known,
        //! such as when the item is being proxied from another incompatible
        //! or emulated system, ApplicationStatus can be used a sensible default fallback.
        ApplicationStatus = 1,
        //! The item describes the status of communication oriented applications,
        //! like an instant messenger or an email client.
        Communications = 2,
        //! The item describes services of the system not seen as a stand alone
        //! application by the user, such as an indicator for the activity
        //! of a disk indexing service.
        SystemServices = 3,
        //! The item describes the state and control of a particular hardware,
        //! such as an indicator of the battery charge or sound card volume control.
        Hardware = 4,
        Reserved = 129,
    };
    Q_ENUM(ItemCategory)
    /**
        Construct a new status notifier item.

        @param id     The application id, see @ref Id.
        @param parent The parent object.
    */
    StatusNotifierItem(QString id, QObject *parent = nullptr);

    ~StatusNotifierItem() override;

    /*!
        @return the id that was specified in the constructor.
    */
    QString id() const;

    /*!
        The application title, see Title.
    */
    void setTitle(const QString &title);

    /*!
        @return the name that describes the application.
    */
    QString title() const;

    /*!
        Sets the status of this item or of the associated application.
        @see ItemStatus
    */
    void setStatus(const QString &status);

    /*!
        @return the status of this item or of the associated application.
        @see ItemStatus
    */
    QString status() const;

    /*!
        Sets the category for this item.
        @see ItemCategory
    */
    void setCategory(const QString &category);

    /*!
        @return the category of this item.
        @see ItemCategory
    */
    QString category() const;

    /*!
        Sets the menu object path.
        @see Menu
    */
    void setMenuPath(const QString &path);

    /*!
        @todo rename to menuPath() or the one above to setMenu()?
        @return the DBus menu path.
        @see Menu
    */
    QDBusObjectPath menu() const;
#if 0
    /*!
        TODO:
        @return if this item supports only a context menu.
        @see Menu
    */
    bool itemIsMenu() const;
#endif
    /*!
        Sets the item icon name, see @ref IconName.
    */
    void setIconByName(const QString &name);

    /*!
        @return the item icon name.
    */
    QString iconName() const;

    /*!
        TODO
    */
    IconPixmapList iconPixmap() const;

    /*!
        TODO
    */
    void setIconByPixmap(const QIcon &icon);

    /*!
        TODO
    */
    QString overlayIconName() const;

    /*!
        TODO
    */
    void setOverlayIconByName(const QString &name);

    /*!
        TODO
    */
    IconPixmapList overlayIconPixmap() const;

    /*!
        TODO
    */
    void setOverlayIconByPixmap(const QIcon &icon);

    /*!
        TODO
    */
    QString attentionIconName() const;

    /*!
        TODO
    */
    void setAttentionIconByName(const QString &name);

    /*!
        TODO
    */
    IconPixmapList attentionIconPixmap() const;

    /*!
        TODO
    */
    void setAttentionIconByPixmap(const QIcon &icon);

    /*!
        TODO
    */
    QString toolTipTitle() const;

    /*!
        TODO
    */
    void setToolTipTitle(const QString &title);

    /*!
        TODO
    */
    QString toolTipSubTitle() const;

    /*!
        TODO
    */
    void setToolTipSubTitle(const QString &subTitle);

    /*!
        TODO
    */
    QString toolTipIconName() const;

    /*!
        TODO
    */
    void setToolTipIconByName(const QString &name);

    /*!
        TODO
    */
    IconPixmapList toolTipIconPixmap() const;

    /*!
        TODO
    */
    void setToolTipIconByPixmap(const QIcon &icon);

    /*!
        @return a ToolTip object instance.
    */
    ToolTip toolTip() const
    {
        ToolTip tt;
        tt.title = mTooltipTitle;
        tt.description = mTooltipSubtitle;
        tt.iconName = mTooltipIconName;
        tt.iconPixmap = mTooltipIcon;
        return tt;
    }

    /*!
        @note We don't take ownership of the menu.
        @param menu The context menu.
    */
    void setContextMenu(QMenu *menu);

    /*!
        @return the context menu associated to this status notifier item.
    */
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

    /*!
        TODO
    */
    void showMessage(const QString &title, const QString &msg, const QString &iconName, int secs);

Q_SIGNALS:
    /*!
        Inform the host application that an activation has been requested.

        This is typically a consequence of user input,
        such as mouse left click over the graphical representation of the item.

        @param position The position in the screen where the user clicked
                        to trigger this signal,
                        QPoint() if it's not the consequence of a mouse click.
    */
    void activateRequested(const QPoint &position);

    /*!
        Inform the host application that a secondary activation has been requested.

        This is typically a consequence of user input,
        such as mouse middle click over the graphical representation of the item.

        @param position The position in the screen where the user clicked
                        to trigger this signal,
                        QPoint() if it's not the consequence of a mouse click.
    */
    void secondaryActivateRequested(const QPoint &position);

    /*!
        Inform the host application that the mouse wheel
        (or another mean of scrolling that the visualization provides) has been used.

        @param delta       The amount of scrolling, can be either positive or negative.
        @param orientation Direction of the scrolling, can be either horizontal or vertical.
    */
    void scrollRequested(int delta, Qt::Orientation orientation);

private:
    void registerToHost();
    IconPixmapList iconToPixmapList(const QIcon &icon);

private Q_SLOTS:
    void onServiceOwnerChanged(const QString &service, const QString &oldOwner,
                               const QString &newOwner);
    void onMenuDestroyed();

private:
    StatusNotifierItemAdaptor *mAdaptor;

    QString mService;
    QString mId;
    QString mTitle;
    QString mStatus;
    QString mCategory;

    // icons
    QString mIconName, mOverlayIconName, mAttentionIconName;
    IconPixmapList mIcon, mOverlayIcon, mAttentionIcon;
    qint64 mIconCacheKey, mOverlayIconCacheKey, mAttentionIconCacheKey;

    // tooltip
    QString mTooltipTitle, mTooltipSubtitle, mTooltipIconName;
    IconPixmapList mTooltipIcon;
    qint64 mTooltipIconCacheKey;

    // menu
    QMenu *mMenu;
    QDBusObjectPath mMenuPath;
    DBusMenuExporter *mMenuExporter;
    QDBusConnection mSessionBus;

    static int mServiceCounter;
};

#endif
