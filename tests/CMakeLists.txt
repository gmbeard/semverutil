add_library(testing OBJECT testing.cpp)
link_libraries(testing semverutil)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(parsing_tests parsing_tests.cpp)
target_link_libraries(parsing_tests testing)
configure_file(parsing_tests_wrapper.sh
    ${CMAKE_CURRENT_BINARY_DIR}/parsing_tests_wrapper.sh
    COPYONLY
    FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)
add_test(NAME parsing_tests
    COMMAND parsing_tests_wrapper.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(cmdline_tests cmdline_tests.cpp)
target_link_libraries(cmdline_tests testing)
add_test(NAME cmdline_tests COMMAND cmdline_tests -M -NP)

add_executable(core_version_validity_tests core_version_validity_tests.cpp)
target_link_libraries(core_version_validity_tests testing)
configure_file(valid_core_version_tests_wrapper.sh
    ${CMAKE_CURRENT_BINARY_DIR}/valid_core_version_tests_wrapper.sh
    COPYONLY
    FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)
configure_file(invalid_core_version_tests_wrapper.sh
    ${CMAKE_CURRENT_BINARY_DIR}/invalid_core_version_tests_wrapper.sh
    COPYONLY
    FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)
add_test(NAME valid_core_version_tests
    COMMAND valid_core_version_tests_wrapper.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_test(NAME invalid_core_version_tests
    COMMAND invalid_core_version_tests_wrapper.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(cmdline_args_tests cmdline_args_tests.cpp)
target_link_libraries(cmdline_args_tests testing)
add_test(NAME cmdline_args_tests COMMAND cmdline_args_tests -M -NP)

add_executable(contracts_tests contracts_tests.cpp)
target_link_libraries(contracts_tests testing)
add_test(NAME contracts_tests COMMAND contracts_tests)

add_executable(format_string_tests format_string_tests.cpp)
target_link_libraries(format_string_tests testing)
add_test(NAME format_string_tests COMMAND format_string_tests)
