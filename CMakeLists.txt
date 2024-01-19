cmake_minimum_required(VERSION 3.18)

set(LIBRARY_NAME "StatusNotifierItemQt")
set(PROJECT_QT_VERSION 5 CACHE STRING "Qt version to use [default: 5]")

project(${LIBRARY_NAME}${PROJECT_QT_VERSION}
    VERSION      0.1.0
    LANGUAGES    CXX
    HOMEPAGE_URL "https://qtilities.github.io/libstatusnotifieritem-qt"
    DESCRIPTION  "Qt implementation of the StatusNotifierItem specification"
)
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR       ON)

option(SNI_QT_WITH_DOC               "Build Doxygen documentation [default: ON]"  ON)
option(SNI_QT_BUILD_EXAMPLE          "Build example application   [default: OFF]" OFF)
option(SNI_QT_EXAMPLE_USE_SYSTEM_LIB "Use SNI Qt system library   [default: OFF]" OFF)
set(SNI_QT_EXPORTS_PREFIX ${LIBRARY_NAME})

configure_file(scripts/${LIBRARY_NAME}.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)
#=======================================================================================================
# Qt
#=======================================================================================================
set(CMAKE_AUTOMOC ON)
find_package(QT NAMES Qt${PROJECT_QT_VERSION})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Widgets)
find_package(DBusMenuQtilities${QT_VERSION_MAJOR} REQUIRED)
#=======================================================================================================
# Source files
#=======================================================================================================
include(GNUInstallDirs)
add_subdirectory(src)
#add_subdirectory(tests)
if(SNI_QT_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
#=======================================================================================================
# Documentation
#=======================================================================================================
if(SNI_QT_WITH_DOC)
    configure_file(scripts/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL doxygen WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    install(
        DIRECTORY   ${CMAKE_BINARY_DIR}/html
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )
endif()
#=======================================================================================================
# CMake Config files
#=======================================================================================================
include(CMakePackageConfigHelpers)

set(SNI_QT_CMAKE_PATH   "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

configure_package_config_file("cmake/${LIBRARY_NAME}Config.cmake.in"
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    PATH_VARS INCLUDE_INSTALL_DIR SNI_QT_CMAKE_PATH
    INSTALL_DESTINATION "${SNI_QT_CMAKE_PATH}"
)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)
#=======================================================================================================
# Installation
#=======================================================================================================
install(
    FILES       "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/pkgconfig"
)
install(
    FILES
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION
        "${SNI_QT_CMAKE_PATH}"
    COMPONENT
        Devel
)
install(
    TARGETS             ${PROJECT_NAME}
    EXPORT              ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    EXPORT      ${PROJECT_NAME}Targets
    DESTINATION ${SNI_QT_CMAKE_PATH}
)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/src/statusnotifieritem_export.h
        ${CMAKE_CURRENT_BINARY_DIR}/src/statusnotifieritem_version.h
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
