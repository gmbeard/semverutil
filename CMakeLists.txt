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

include(Support)
include(GNUInstallDirs)

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

install(TARGETS semverutil
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
