add_library(
    semverutilobj OBJECT
    cmdline.cpp
    semver.cpp
    utils.cpp
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
