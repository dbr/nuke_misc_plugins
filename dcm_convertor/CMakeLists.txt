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

# Locate Houdini
find_package(Houdini REQUIRED)
message("Build against Houdini at ${Houdini_LIBRARY_DIR}")

# Include Nuke headers
include_directories(
    "/Applications/Graphics/3Delight-9.0.132/include"
    ${Houdini_INCLUDE_DIR}
)

# Add plugin as shared library (.so on linux, .dylib on OS X)
add_executable(dcm_convertor
    src/dcm_convertor.mm
)

# Link against Nuke's libraries
target_link_libraries(dcm_convertor
    ${Houdini_LIBRARIES}
    /Applications/Graphics/3Delight-9.0.132/lib/lib3delight.dylib
)

# Prevent lib prefix on filesnames
set_target_properties(dcm_convertor
    PROPERTIES
    PREFIX ""
    COMPILE_FLAGS "${Houdini_COMPILE_FLAGS} -fobjc-gc-only"
    LINK_FLAGS "${Houdini_LINK_FLAGS}"
)

# Once built, copy the library into install location (e.g "executable-prefix/lib/nuke/6.2")
install(TARGETS dcm_convertor
        DESTINATION ${CMAKE_INSTALL_EXEC_PREFIX}/bin)
