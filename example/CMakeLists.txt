project(example
    VERSION   0.1.0
    LANGUAGES CXX
)
if(SNI_QT_EXAMPLE_USE_SYSTEM_LIB)
    find_package(StatusNotifierItemQt${QT_VERSION_MAJOR})
else()
    include_directories(${CMAKE_BINARY_DIR}/src) #include "statusnotifieritem_export.h"
endif()

set(PROJECT_SOURCES example.cpp)
source_group("" FILES ${PROJECT_SOURCES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Widgets
    Qt::DBus
)
target_link_libraries(${PROJECT_NAME} PRIVATE StatusNotifierItemQt${QT_VERSION_MAJOR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
