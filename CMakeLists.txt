cmake_minimum_required(VERSION 3.22)

file(READ ".versioning/current" current_version)
string(STRIP "${current_version}" current_version)

project(
    semverutil
    LANGUAGES CXX
    VERSION ${current_version}
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
message(STATUS "Project version: ${PROJECT_VERSION}")

option(
    SEMVERUTIL_ENABLE_TESTS
    "Enable tests for ${PROJECT_NAME}"
    OFF
)

option(
    SEMVERUTIL_INSTALL_DEVEL
    "Install target includes the ${PROJECT_NAME} development \
headers and libs"
    OFF
)

include(Support)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Werror -Wextra -Wconversion -Wpedantic)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
else()
    add_compile_options(-g)
endif()

add_subdirectory(src)

if(SEMVERUTIL_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS semverutilbin
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(SEMVERUTIL_INSTALL_DEVEL)
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/SemverutilConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Semverutil
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/SemverutilConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
    )

    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PATTERN "semverutil/cmdline.hpp" EXCLUDE
    )

    install(FILES ${PROJECT_BINARY_DIR}/config/semverutil/config.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/semverutil/
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/SemverutilConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/SemverutilConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Semverutil
    )

    install(TARGETS semverutil
        EXPORT SemverutilTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(EXPORT SemverutilTargets
        FILE SemverutilTargets.cmake
        NAMESPACE Semverutil::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Semverutil
    )
endif()
