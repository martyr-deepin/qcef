cmake_minimum_required(VERSION 3.1)
project(browser-demo)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti, Disable real-time type information
  # -fno-exceptions Disable exceptions
  # -fsanitize=address Enable memory sanitize plugin
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -fno-rtti -fno-exceptions")
  # -fsanitize=address, Enable memory address sanitizer.
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  # -fsanitize=leak, Enable memory leak sanitizer.
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  # -fsanitize=memory, Enable detecting uninitited memory sanitizer.
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory "
  #    "-fsanitize-memory-track-origins -fPIC -pie -fno-omit-frame-pointer")

  # Enable Qt builtin debug mode
  add_definitions("-DQT_MESSAGELOGCONTEXT")
else()
  # -Wl, -O2 Enable linker optimizations
  #-Wl, --gc-sections Remove unused code resulting from -fdsta-sections and -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections -fno-rtti -Wno-unused-result")
endif()


find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#pkg_search_module(LIBQCEF REQUIRED libqcef)
include_directories(AFTER ${LIBQCEF_INCLUDE_DIRS})

#set(LINK_LIBS
#    Qt5::Gui
#    Qt5::WebChannel
#    Qt5::Widgets
#    ${LIBQCEF_LDFLAGS}
#    )

# Use local libqcef version instead of system.
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
    channel.cpp channel.h)

add_executable(browser-demo
               ${BROWSER_DEMO_FILES})
target_link_libraries(browser-demo ${LINK_LIBS})