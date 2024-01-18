configure_file(statusnotifieritem_export.h.in  ${CMAKE_CURRENT_BINARY_DIR}/statusnotifieritem_export.h  @ONLY)
configure_file(statusnotifieritem_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/statusnotifieritem_version.h @ONLY)

set(PROJECT_SOURCES
    dbustypes.h
    dbustypes.cpp
    statusnotifieritem.h
    statusnotifieritem_p.h
    statusnotifieritem.cpp
    org.kde.StatusNotifierItem.xml
)
qt_add_dbus_adaptor(PROJECT_SOURCES
    org.kde.StatusNotifierItem.xml
    statusnotifieritem.h
    StatusNotifierItem
)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
source_group("" FILES ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_include_directories(${PROJECT_NAME} INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Widgets
    Qt::DBus
    DBusMenuQtilities${QT_VERSION_MAJOR}
)
install(
    DIRECTORY              .
    DESTINATION            "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    FILES_MATCHING PATTERN "*.h"
    PATTERN                "*_p.h" EXCLUDE
)
