file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/config/semverutil")
configure_file(
    config.hpp.in
    "${PROJECT_BINARY_DIR}/config/semverutil/config.hpp"
)

add_library(
    semverutilobj OBJECT
    cmdline.cpp
    detail/semver.cpp
    semver.cpp
    utils.cpp
)

target_include_directories(
    semverutilobj
    PUBLIC
    "${PROJECT_BINARY_DIR}/config"
)

add_executable(
    semverutil
    main.cpp
)

target_link_libraries(
    semverutil
    PRIVATE
    semverutilobj
)
