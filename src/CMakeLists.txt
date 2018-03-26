project(qcef CXX C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

set(QCEF_VERSION_MAJOR 1)
set(QCEF_VERSION_MINOR 1)
set(QCEF_VERSION_MICRO 4)

if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,'\$ORIGIN'/qcef")
endif ()

if (NOT DEFINED QCEF_INSTALL_PATH)
  set(QCEF_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/lib")
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(QCEF_OVERRIDE_PATH "${CMAKE_CURRENT_BINARY_DIR}/qcef")
  else ()
    set(QCEF_OVERRIDE_PATH "${QCEF_INSTALL_PATH}/qcef")
  endif ()
else ()
  set(QCEF_OVERRIDE_PATH "${QCEF_INSTALL_PATH}/qcef")
endif ()

# Define absolute path to qcef module, i.e. /usr/lib/qcef.
# See https://github.com/chromiumembedded/cef/pull/4.
add_definitions("-DQCEF_OVERRIDE_PATH=\"${QCEF_OVERRIDE_PATH}\"")

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti, Disable real-time type information
  # -fno-exceptions Disable exceptions
  # -fsanitize=address Enable memory sanitize plugin
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -fno-rtti -fno-exceptions")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  # -fsanitize=address, Enable memory address sanitizer.
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  # -fsanitize=leak, Enable memory leak sanitizer.
  #   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  # -fsanitize=memory, Enable detecting uninitited memory sanitizer.
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory "
  #    "-fsanitize-memory-track-origins -fPIC -pie -fno-omit-frame-pointer")

  # Enable Qt builtin debug mode
  add_definitions("-DQT_MESSAGELOGCONTEXT")
else ()
  # -Wl, -O2 Enable linker optimizations
  #-Wl, --gc-sections Remove unused code resulting from -fdsta-sections and -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
  if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-O1 -Wl,--gc-sections")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-O1 -Wl,--gc-sections")
  endif ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fno-rtti -Wno-unused-result -fPIC")
endif ()

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebChannel REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5LinguistTools)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add the include directories for the Qt 5 Gui module private components to
# the compile lines.
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

set(QT_LIBS
    Qt5::DBus
    Qt5::Gui
    Qt5::Network
    Qt5::WebChannel
    Qt5::Widgets
    Qt5::X11Extras
    )

set(LINK_LIBS
    ${QT_LIBS}

    -lX11
    )


set(BASE_FILES
    base/file_util.cpp
    base/file_util.h
    )

set(CORE_FILES
    core/qcef_app.cpp
    core/qcef_app.h
    core/qcef_browser_transport.cpp
    core/qcef_browser_transport.h
    core/qcef_client_handler.cpp
    core/qcef_client_handler.h
    core/qcef_context.cpp
    core/qcef_context.h
    core/qcef_cookie_store.cpp
    core/qcef_cookie_store.h
    core/qcef_core_export.h
    core/qcef_dialog_handler.cpp
    core/qcef_dialog_handler.h
    core/qcef_global_settings.cpp
    core/qcef_global_settings.h
    core/qcef_key_event_map.cpp
    core/qcef_key_event_map.h
    core/qcef_message_pump_handler.cpp
    core/qcef_message_pump_handler.h
    core/qcef_notification_constructor.cpp
    core/qcef_notification_constructor.h
    core/qcef_print_handler.cpp
    core/qcef_print_handler.h
    core/qcef_renderer_handler.cpp
    core/qcef_renderer_handler.h
    core/qcef_renderer_transport_handler.cpp
    core/qcef_renderer_transport_handler.h
    core/qcef_scheme_handler.h
    core/qcef_scheme_handler_factory.cpp
    core/qcef_scheme_handler_factory.h
    core/qcef_string_visitor.cpp
    core/qcef_string_visitor.h
    core/qcef_sync_method.h
    core/qcef_sync_method_handler.cpp
    core/qcef_sync_method_handler.h
    core/qcef_util.cpp
    core/qcef_util.h
    core/qcef_web_channel_consts.h
    core/qcef_x11_util.cpp
    core/qcef_x11_util.h
    )
if (EMBED_WEBCHANNEL)
  set(CORE_FILES
      ${CORE_FILES}
      third_party/webchannel/resources.qrc)
endif ()

set(CORE_EXPORT_FILES
    core/qcef_context.h
    core/qcef_cookie_store.h
    core/qcef_core_export.h
    core/qcef_global_settings.h
    core/qcef_scheme_handler.h
    core/qcef_sync_method.h
    core/qcef_util.h
    )

set(WIDGETS_FILES

    widgets/qcef_browser_event_delegate.h
    widgets/qcef_browser_event_delegate.cpp
    widgets/qcef_browser_event_delegate_p.h
    widgets/qcef_client_handler_delegate.cpp
    widgets/qcef_client_handler_delegate.h
    widgets/qcef_native_event_filter.cpp
    widgets/qcef_native_event_filter.h
    widgets/qcef_notification_service.cpp
    widgets/qcef_notification_service.h
    widgets/qcef_ssl_status.h
    widgets/qcef_web_page.cpp
    widgets/qcef_web_page.h
    widgets/qcef_web_settings.cpp
    widgets/qcef_web_settings.h
    widgets/qcef_web_view.cpp
    widgets/qcef_web_view.h
    widgets/qcef_widgets_export.h
    )

set(WIDGETS_EXPORT_FILES
    widgets/qcef_browser_event_delegate.h
    widgets/qcef_notification_service.h
    widgets/qcef_ssl_status.h
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
                   "${CEF_TARGET_OUT_DIR}/qcef/libcef.so"
                   "${CEF_TARGET_OUT_DIR}/qcef/libcef.so")

add_custom_command(OUTPUT _copy-cef
                   COMMAND echo "COPY CEF!!!!"
                   COMMAND rm -rvf qcef/
                   COMMAND mkdir -pv qcef/
                   COMMAND tar xvf "${CMAKE_CURRENT_SOURCE_DIR}/../cef/cef.${PROJECT_ARCH}.tar.gz"
                   COMMAND mv -vf
                   Release/chrome-sandbox
                   Release/natives_blob.bin
                   Release/snapshot_blob.bin
                   Release/libcef.so
                   Resources/locales
                   Resources/cef.pak
                   Resources/cef_100_percent.pak
                   Resources/cef_200_percent.pak
                   Resources/cef_extensions.pak
                   Resources/devtools_resources.pak
                   Resources/icudtl.dat
                   qcef/
                   COMMAND rm -rf Release Resources
                   WORKING_DIRECTORY "${CEF_TARGET_OUT_DIR}"
                   VERBATIM
                   )
add_custom_target(copy-cef DEPENDS _copy-cef)

# Executable target.
add_library(${CEF_TARGET} SHARED
            ${BASE_FILES}
            ${CORE_FILES}
            ${WIDGETS_FILES}
            )
SET_LIBRARY_TARGET_PROPERTIES(${CEF_TARGET})
add_dependencies(${CEF_TARGET}
                 copy-cef
                 libcef_dll_wrapper
                 )
target_link_libraries(${CEF_TARGET}
                      ${LINK_LIBS}
                      libcef_lib
                      libcef_dll_wrapper)

# Set rpath so that libraries can be placed next to the executable.
set_target_properties(${CEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
set_target_properties(${CEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(${CEF_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CEF_TARGET_OUT_DIR})
set_target_properties(${CEF_TARGET}
                      PROPERTIES
                      VERSION
                      "${QCEF_VERSION_MAJOR}.${QCEF_VERSION_MINOR}.${QCEF_VERSION_MICRO}"
                      SOVERSION
                      ${QCEF_VERSION_MAJOR})

# PkgConfig file.
set(QCEF_PKG_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/libqcef.pc)
configure_file(data/libqcef.pc.in ${QCEF_PKG_CONFIG_FILE})

if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_executable(notification-test
                 tests/notification_test.cpp

                 tests/notification_window.cpp
                 tests/notification_window.h
                 widgets/qcef_notification_service.cpp
                 widgets/qcef_notification_service.h
                 )
  target_link_libraries(notification-test ${LINK_LIBS})

  add_executable(xevent-tracer
                 tests/xevent-tracer.c)
  target_link_libraries(xevent-tracer ${LINK_LIBS})

  add_executable(qt-version tests/qt_version_test.cpp)
  target_link_libraries(qt-version ${QT_LIBS})
endif ()

## Installation
install(TARGETS "${CEF_TARGET}"
        DESTINATION "${QCEF_INSTALL_PATH}")

# Install header files.
install(FILES ${CORE_EXPORT_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/qcef)
install(FILES ${WIDGETS_EXPORT_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/qcef)

# Install pkgconfig files.
install(FILES ${QCEF_PKG_CONFIG_FILE}
        DESTINATION ${QCEF_INSTALL_PATH}/pkgconfig/)

# Install cef binary files.
install(DIRECTORY "${CEF_TARGET_OUT_DIR}/qcef"
        DESTINATION "${QCEF_INSTALL_PATH}")

# Fix sandbox permission.
install(FILES "${CEF_TARGET_OUT_DIR}/qcef/chrome-sandbox"
        DESTINATION "${QCEF_OVERRIDE_PATH}"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
        SETUID SETGID)
