file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/config/semverutil")
configure_file(
    config.hpp.in
    "${PROJECT_BINARY_DIR}/config/semverutil/config.hpp"
)

add_library(
    semverutil
    cmdline.cpp
    detail/semver.cpp
    format.cpp
    semver.cpp
    utils.cpp
)

target_include_directories(
    semverutil
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/config>
    $<INSTALL_INTERFACE:include>
)

add_executable(
    semverutilbin
    main.cpp
)

target_link_options(
    semverutilbin
    PRIVATE
    $<$<BOOL:${SEMVERUTIL_STATIC_BINARY}>:-static -static-libgcc -static-libstdc++>
)

target_link_libraries(
    semverutilbin
    PRIVATE
    semverutil
)

set_target_properties(
    semverutilbin
    PROPERTIES
        OUTPUT_NAME semverutil$<$<BOOL:${SEMVERUTIL_STATIC_BINARY}>:-static>
)

