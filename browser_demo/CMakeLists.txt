cmake_minimum_required(VERSION 3.1)
project(browser-demo)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

pkg_search_module(LIBQCEF REQUIRED libqcef)
include_directories(AFTER ${LIBQCEF_INCLUDE_DIRS})

set(LINK_LIBS
    Qt5::Gui
    Qt5::Widgets
    ${LIBQCEF_LIBRARIES}
    )

set(BROWSER_DEMO_FILES
    main.cpp
    browser_window.cpp
    browser_window.h)

add_executable(browser-demo
               ${CORE_HEADER_FILES}
               ${BROWSER_DEMO_FILES}
               )
SET_EXECUTABLE_TARGET_PROPERTIES(browser-demo)
target_link_libraries(browser-demo ${LINK_LIBS})