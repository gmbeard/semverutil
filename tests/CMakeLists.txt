add_library(testing OBJECT testing.cpp)
link_libraries(testing semverutilobj)
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
