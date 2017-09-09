cmake_minimum_required(VERSION 3.1)
project(browser-demo)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

pkg_search_module(LIBQCEF REQUIRED libqcef)
include_directories(AFTER ${LIBQCEF_INCLUDE_DIRS})

set(LINK_LIBS
    Qt5::Gui
    Qt5::WebChannel
    Qt5::Widgets
    ${LIBQCEF_LIBRARIES}
    )

set(BROWSER_DEMO_FILES
    main.cpp
    browser_address_edit.cpp
    browser_address_edit.h
    browser_event_delegate.cpp
    browser_event_delegate.h
    browser_tab_bar.cpp
    browser_tab_bar.h
    browser_tab_widget.cpp
    browser_tab_widget.h
    browser_window.cpp
    browser_window.h
    sync_methods.cpp
    sync_methods.h

    images/images.qrc
    resources/resources.qrc
    )

add_executable(browser-demo
               ${BROWSER_DEMO_FILES})
target_link_libraries(browser-demo ${LINK_LIBS})
