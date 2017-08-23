cmake_minimum_required(VERSION 3.1)
project(qcef CXX C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Define absolute path to qcef module, i.e. /usr/lib/qcef.
# See https://github.com/chromiumembedded/cef/pull/4.
if (NOT DEFINED QCEF_OVERRIDE_PATH)
  set(QCEF_OVERRIDE_PATH "${CMAKE_CURRENT_BINARY_DIR}/qcef")
endif()
add_definitions("-DQCEF_OVERRIDE_PATH=\"${QCEF_OVERRIDE_PATH}\"")

if (NOT DEFINED LIB_INSTALL_DIR)
  set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti, Disable real-time type information
  # -fno-exceptions Disable exceptions
  # -fsanitize=address Enable memory sanitize plugin
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -Wno-unused-parameter -fno-rtti -fno-exceptions")
  # -fsanitize=address, Enable memory address sanitizer.
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  # -fsanitize=leak, Enable memory leak sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
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
find_package(Qt5Network REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5LinguistTools)

pkg_search_module(GLIB2 REQUIRED glib-2.0)
pkg_search_module(GTK2 REQUIRED gtk+-2.0)
include_directories(AFTER ${GLIB2_INCLUDE_DIRS})
include_directories(AFTER ${GTK2_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_LIBS
    Qt5::Gui
    Qt5::Network
    Qt5::WebChannel
    Qt5::Widgets
    Qt5::X11Extras
    )

set(LINK_LIBS
    ${QT_LIBS}

    ${GLIB2_LIBRARIES}
    ${GTK2_LIBRARIES}
    -lX11
    )

set(DATA_FILES
    data/libqcef.pc
    )

set(BASE_FILES
    base/file_util.cpp
    base/file_util.h
    )

set(CORE_FILES
    core/qcef_app.cc
    core/qcef_app.h
    core/qcef_browser_transport.cpp
    core/qcef_browser_transport.h
    core/qcef_client_handler.cc
    core/qcef_client_handler.h
    core/qcef_context.cpp
    core/qcef_context.h
    core/qcef_cookie_store.cpp
    core/qcef_cookie_store.h
    core/qcef_core_export.h
    core/qcef_global_settings.cpp
    core/qcef_global_settings.h
    core/qcef_renderer_handler.cpp
    core/qcef_renderer_handler.h
    core/qcef_renderer_transport_handler.cpp
    core/qcef_renderer_transport_handler.h
    core/qcef_scheme_handler.h
    core/qcef_scheme_handler_factory.cpp
    core/qcef_scheme_handler_factory.h
    core/qcef_util.cpp
    core/qcef_util.h
    core/qcef_web_channel_consts.h
    )

set(CORE_EXPORT_FILES
    core/qcef_context.h
    core/qcef_cookie_store.h
    core/qcef_core_export.h
    core/qcef_global_settings.h
    core/qcef_scheme_handler.h
    )

set(WIDGETS_FILES

    widgets/qcef_client_handler_delegate.cpp
    widgets/qcef_client_handler_delegate.h
    widgets/qcef_web_page.cpp
    widgets/qcef_web_page.h
    widgets/qcef_web_settings.cpp
    widgets/qcef_web_settings.h
    widgets/qcef_web_view.cpp
    widgets/qcef_web_view.h
    widgets/qcef_widgets_export.h
    )

set(WIDGETS_EXPORT_FILES
    widgets/qcef_web_page.h
    widgets/qcef_web_settings.h
    widgets/qcef_web_view.h
    widgets/qcef_widgets_export.h
    )

## Libraries and executables.
# Target executable names.
set(CEF_TARGET "qcef")
set(CEF_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib"
                   "${CEF_TARGET_OUT_DIR}/libcef.so"
                   "${CEF_TARGET_OUT_DIR}/libcef.so")

# Executable target.
add_library(${CEF_TARGET} SHARED
            ${BASE_FILES}
            ${CORE_FILES}
            ${WIDGETS_FILES}
            )
SET_LIBRARY_TARGET_PROPERTIES(${CEF_TARGET})
add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
target_link_libraries(${CEF_TARGET}
                      ${LINK_LIBS}
                      libcef_lib
                      libcef_dll_wrapper)

# Set rpath so that libraries can be placed next to the executable.
set_target_properties(${CEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
set_target_properties(${CEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(${CEF_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
set_target_properties(${CEF_TARGET}
                      PROPERTIES VERSION 1.0
                      SOVERSION 1)

# Extract CEF binary files
macro(EXTRACT_CEF_TARBALL target)
  add_custom_command(
    TARGET ${target}
    PRE_BUILD
    COMMAND rm -rvf qcef/
    COMMAND mkdir -pv qcef/
    COMMAND tar xvf "${CMAKE_SOURCE_DIR}/cef/cef.${PROJECT_ARCH}.tar.gz"
    COMMAND mv -vf Release/libcef.so .
    COMMAND mv -vf Release/chrome-sandbox Release/natives_blob.bin Release/snapshot_blob.bin qcef/
    COMMAND rm -rf Release/
    COMMAND rm -rf locales/
    COMMAND mv -vf Resources/locales Resources/cef.pak
        Resources/cef_100_percent.pak Resources/cef_200_percent.pak
        Resources/cef_extensions.pak Resources/devtools_resources.pak
        Resources/icudtl.dat
        qcef/
    COMMAND rm -rf Resources/
    WORKING_DIRECTORY "${CEF_TARGET_OUT_DIR}"
    VERBATIM
  )
endmacro()
# Add logical library
add_library(CEF_TARBALL base/macros.h)
add_dependencies(${CEF_TARGET} CEF_TARBALL)
# Copy binary and resource files to the target output directory.
EXTRACT_CEF_TARBALL("CEF_TARBALL")

## Installation
install(TARGETS "${CEF_TARGET}"
        DESTINATION "${LIB_INSTALL_DIR}")
install(FILES "${CEF_TARGET_OUT_DIR}/libcef.so"
        DESTINATION "${LIB_INSTALL_DIR}")

# Install header files.
install(FILES ${CORE_EXPORT_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/qcef)
install(FILES ${WIDGETS_EXPORT_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/qcef)

# Install pkgconfig files.
install(FILES ${DATA_FILES}
        DESTINATION ${LIB_INSTALL_DIR}/pkgconfig/)

# Install cef binary files.
install(DIRECTORY "${CEF_TARGET_OUT_DIR}/qcef"
        DESTINATION "${LIB_INSTALL_DIR}")

# Fix sandbox permission.
install(FILES "${CEF_TARGET_OUT_DIR}/qcef/chrome-sandbox"
        DESTINATION "${LIB_INSTALL_DIR}/qcef"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
        SETUID SETGID)
