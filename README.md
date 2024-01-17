# libstatusnotifieritem-qt

[![CI]](https://github.com/qtilities/libstatusnotifieritem-qt/actions/workflows/build.yml)

## Overview

This library provides a Qt implementation of the Freedesktop' [StatusNotifierItem] specification.

## Dependencies

### Runtime

- Qt5/6 base
- [DBusMenuQtilities] library

### Build

- CMake
- Git (optional, to pull latest VCS checkouts)

## Documentation

By default documentation is generated with Doxygen.
You can disable documentation generation by passing `-D SNI_QT_WITH_DOC=OFF` to CMake.

## Packages

[![Packages]](https://repology.org/project/libstatusnotifieritem-qt/versions)

## Licenses

- StatusNotifierItemQt is licensed under the [LGPL-2.1-or-later] license.


[CI]:                 https://github.com/qtilities/libstatusnotifieritem-qt/actions/workflows/build.yml/badge.svg
[DBusMenuQtilities]:  https://github.com/qtilities/libdbusmenu-qtilities/
[LGPL-2.1-or-later]:  COPYING
[Packages]:           https://repology.org/badge/vertical-allrepos/libstatusnotifieritem-qt.svg
[StatusNotifierItem]: https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem/
