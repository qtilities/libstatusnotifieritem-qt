# libStatusNotifierItemQt

Qt implementation of the Freedesktop' StatusNotifierItem specification.<br />
From the [homepage] content:

This specification defines the management of visual items,
usually icons used for reporting the status of an application to the user
or provide a quick access to common actions performed by that application.
It is intended to be complementary but not directly related with
the Freedesktop's Desktop Notifications specification and is aimed as a replacement
to the Freedesktop System tray specification, tough more model-view oriented,
giving more freedom to the workspace how to graphically represent the items
coherent to its visual style language.

This specification does not define what the aspect of the Notification Items
will be, this is strictly implementation specific.

Example use cases will include:
- Mixer volume control.
- Battery indicator.
- Instant messenger on-line status.
- Generic application information and actions, like a media player controls.

## Basic Design

The Status Notifier Item system relies on inter-process communication via DBus
and is composed by three parts:
- StatusNotifierItem: each application that wants to utilize the system registers
  its own StatusNotifierItem to the session bus.
- StatusNotifierWatcher: service used to keep track of every active instance
  of StatusNotifierItem, used by StatusNotifierHost to get the list of all items.
  It also notifies it when items are added and removed.
- StatusNotifierHost: an application that wants to provide a visualization
  of the existing StatusNotifierItem instances must register a StatusNotifierHost
  in the session bus.

[Markup](https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/Markup/)<br />
[Icons](https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/Icons/)<br />

## Previous Discussions

- <http://www.notmart.org/misc/statusnotifieritem/index.html>
- <https://wiki.ubuntu.com/DesktopExperienceTeam/ApplicationIndicators>

## Dependencies

### Runtime

- Qt5/6 base
- [DBusMenuQtilities] library

### Build

- CMake
- Git (optional, to pull latest VCS checkouts)

## Licenses

- StatusNotifierItemQt is licensed under the [LGPL-2.1-or-later] license.


[DBusMenuQtilities]: https://github.com/qtilities/libdbusmenu-qtilities/
[LGPL-2.1-or-later]: https://spdx.org/licenses/LGPL-2.1-or-later.html
[Homepage]:          https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/
