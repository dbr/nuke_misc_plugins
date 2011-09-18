cmake_minimum_required(VERSION 2.8)

# Pick up share/cmake/ modules
# TODO: Append instead of overriding
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/share/cmake)

# If CMAKE_INSTALL_EXEC_PREFIX is not specified, install binaries
# directly into the regular install prefix
if(NOT CMAKE_INSTALL_EXEC_PREFIX)
    message("Exec prefix not specified, defaulting to ${CMAKE_INSTALL_PREFIX}")
    set(CMAKE_INSTALL_EXEC_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

# Locate Nuke or error
find_package(Nuke REQUIRED)
message("Build Nuke plugins against ${Nuke_LIBRARY_DIR}")

# Include Nuke headers
include_directories(
    ${Nuke_INCLUDE_DIR}
)

# Add plugin as shared library (.so on linux, .dylib on OS X)
add_library(ratReaderDeep SHARED
    src/ratReaderDeep.cpp
)

# Link against Nuke's libraries
target_link_libraries(ratReaderDeep
    ${Nuke_LIBRARIES}
)

# Prevent lib prefix on filesnames
set_target_properties(ratReaderDeep
    PROPERTIES
    PREFIX ""
)

# Once built, copy the library into install location (e.g "executable-prefix/lib/nuke/6.2")
install(TARGETS ratReaderDeep
        DESTINATION ${CMAKE_INSTALL_EXEC_PREFIX}/lib/nuke${Nuke_DDImageVersion})
