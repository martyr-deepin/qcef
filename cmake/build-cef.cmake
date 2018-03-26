
# Build cef library and copy to cef/ folder.
add_custom_command(OUTPUT _build-cef
                   COMMAND echo 'BEGIN build-cef'
                   COMMAND make
                   COMMAND rm -rf out/
                   COMMAND make build-cef-tarball
                   COMMAND cp -rf out/cef_minimal/* ../cef
                   COMMAND echo 'END build-cef'
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/qcef-chromium)
add_custom_target(build-cef DEPENDS _build-cef)