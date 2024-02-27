/*
    SPDX-FileCopyrightText:  2015 LXQt team, 2024 Qtilities team
    SPDX-License-Identifier: LGPL-2.1-or-later

    This file is part of the statusnotifieritem-qt library
    Authors:
        Paulo Lieuthier <paulolieuthier@gmail.com>
*/
#ifndef STATUS_NOTIFIER_ITEM_H
#define STATUS_NOTIFIER_ITEM_H

#include "statusnotifieritem_export.h"

#include <QIcon>
#include <QObject>
#include <QPoint>
#include <QString>

#include <memory>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class StatusNotifierItemPrivate;
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

    friend class StatusNotifierItemPrivate;
    friend class StatusNotifierItemDBus;

public:
    //! Describes the status of this item or of the associated application.
    //! The allowed values for the Status property are:
    enum SNIStatus {
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
    Q_ENUM(SNIStatus)

    //! Describes the category of this item.
    //! The allowed values for the Category property are:
    enum SNICategory {
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
    Q_ENUM(SNICategory)

    /**
        Construct a new status notifier item.

        @param id     The application id.
        @param parent The parent object.
    */
    StatusNotifierItem(QString id, QObject *parent = nullptr);

    ~StatusNotifierItem() override;

    /*!
        @return the id that was specified in the constructor.
    */
    QString id() const;

    /*!
        Sets the category for this item.
        @see SNICategory
    */
    void setCategory(SNICategory category);

    /*!
        @return the category of this item.
        @see SNICategory
    */
    SNICategory category() const;

    /*!
        Sets the status of this item or of the associated application.
        @see SNIStatus
    */
    void setStatus(SNIStatus status);

    /*!
        @return the status of this item or of the associated application.
        @see SNIStatus
    */
    SNIStatus status() const;

    /*!
        The application title, @see Title.
    */
    void setTitle(const QString &title);

    /*!
        @return the name that describes the application.
    */
    QString title() const;

    /*!
        Sets a new main icon for the system tray.

        @param name it must be a QIcon::fromTheme compatible name,
        this is the preferred way to set an icon
    */
    void setIconByName(const QString &name);

    /*!
        @return the name of the main icon to be displayed.
    */
    QString iconName() const;

    /*!
        Sets a new main icon for the system tray

        @param icon the icon.
        @note Use setIcon(const QString&) when possible
    */
    void setIconByPixmap(const QIcon &icon);

    /*!
        @return the icon.
    */
    QIcon iconPixmap() const;

    /*!
        Sets an icon to be used as overlay for the main one

        @param name icon name.
        If name is an empty QString() and overlayIconPixmap() is empty too, the icon will be removed.
    */
    void setOverlayIconByName(const QString &name);

    /*!
        @return the name of the icon to be used as overlay for the main one.
    */
    QString overlayIconName() const;

    /*!
        Sets an icon to be used as overlay for the main one.
        setOverlayIconByPixmap(QIcon()) will remove the overlay when overlayIconName() is empty too.

        @param icon overlay icon.
        @note Use setOverlayIcon(const QString&) when possible.
    */
    void setOverlayIconByPixmap(const QIcon &icon);

    /*!
        @return the overlay icon.
    */
    QIcon overlayIconPixmap() const;

    /*!
        Sets a new icon that should be used when the application wants to request attention
        (usually the systemtray will blink between this icon and the main one).

        @param name QIcon::fromTheme() compatible name of icon to use.
    */
    void setAttentionIconByName(const QString &name);

    /*!
        @return the name of the icon to be displayed when the application is requesting the user's attention.

        If attentionImage() is not empty, this will always return an empty string.
    */
    QString attentionIconName() const;

    /*!
        Sets the pixmap of the requesting attention icon.
        @note Use setAttentionIcon(const QString) instead when possible.

        @param icon QIcon to use for requesting attention.
    */
    void setAttentionIconByPixmap(const QIcon &icon);

    /*!
        @return the requesting attention icon.
    */
    QIcon attentionIconPixmap() const;
#if 0
    /*!
        Sets a movie as the requesting attention icon.
        This overrides anything set in setAttentionIcon().
    */
    void setAttentionMovieByName(const QString &name);

    /*!
        @return the name of the movie to be displayed when the application is requesting the user attention.
    */
    QString attentionMovieName() const;
#endif
    /*!
        Sets a new toolTip or this icon, a toolTip is composed of an icon, a title and a text,
        all fields are optional.

        @param iconName a QIcon::fromTheme compatible name for the tootip icon
        @param title    tootip title
        @param subTitle subtitle for the toolTip
    */
    void setToolTip(const QString &iconName, const QString &title, const QString &subTitle);

    /*!
        Sets a new toolTip or this status notifier item.
        This is an overloaded member provided for convenience.

        @param icon     the tootip icon
        @param title    tootip title
        @param subTitle subtitle for the toolTip
    */
    void setToolTip(const QIcon &icon, const QString &title, const QString &subTitle);
    /*!
        Sets a new icon for the tooltip.

        @param name the name for the icon.
    */
    void setToolTipIconByName(const QString &name);

    /*!
        @return the name of the tooltip icon.
        If toolTipImage() is not empty, this will always return an empty string.
    */
    QString toolTipIconName() const;

    /*!
        Sets a new icon for the tooltip.

        @note Use setToolTipIconByName(QString) if possible.
        @param icon the tooltip icon.
    */
    void setToolTipIconByPixmap(const QIcon &icon);

    /*!
        @return the tooltip icon.
    */
    QIcon toolTipIconPixmap() const;

    /*!
        Sets a new title for the tooltip.
    */
    void setToolTipTitle(const QString &title);

    /*!
        @return the title of the tooltip.
    */
    QString toolTipTitle() const;

    /*!
        Sets a new subtitle for the tooltip.
    */
    void setToolTipSubTitle(const QString &subTitle);

    /*!
        @return the subtitle of the tooltip.
    */
    QString toolTipSubTitle() const;

    /*!
        Sets a new context menu for this StatusNotifierItem.

        The menu will be shown with a contextMenu(int, int) call by the systemtray over D-Bus.
        Usually you don't need to call this unless you want to use a custom QMenu subclass as context menu.

        @note The StatusNotifierItem instance takes ownership of the menu,
        and will delete it upon its destruction.

        @param menu The context menu.
    */
    void setContextMenu(QMenu *menu);

    /*!
        @return the context menu associated to this status notifier item.
    */
    QMenu* contextMenu() const;

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
    std::unique_ptr<StatusNotifierItemPrivate> const d;
};

#endif
