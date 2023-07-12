message(STATUS "Checking compiler support...")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

file(
    WRITE
    ${CMAKE_CURRENT_BINARY_DIR}/support/array_three_way_compare.cpp
    "#include <array>
#include <compare>

int main()
{
    std::array<int, 4> a = { 1, 2, 3, 4 };
    std::array<int, 4> b = { 4, 3, 2, 1 };
    static_cast<void>(a <=> b);
    return 0;
}"
)

try_compile(
    SEMVERUTIL_HAS_ARRAY_3_WAY_COMPARE
    ${CMAKE_CURRENT_BINARY_DIR}/support
    ${CMAKE_CURRENT_BINARY_DIR}/support/array_three_way_compare.cpp
    OUTPUT_VARIABLE compile_output
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)
message(DEBUG "${compile_output}")
message(STATUS "std::array supports 3-way compare: " ${SEMVERUTIL_HAS_ARRAY_3_WAY_COMPARE})

file(
    WRITE
    ${CMAKE_CURRENT_BINARY_DIR}/support/string_three_way_compare.cpp
    "#include <string>
#include <compare>

int main()
{
    std::string a { \"Hello\" };
    std::string b { \"World\" };
    static_cast<void>(a <=> b);
    return 0;
}"
)

try_compile(
    SEMVERUTIL_HAS_STRING_3_WAY_COMPARE
    ${CMAKE_CURRENT_BINARY_DIR}/support
    ${CMAKE_CURRENT_BINARY_DIR}/support/string_three_way_compare.cpp
    OUTPUT_VARIABLE compile_output
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)
message(DEBUG "${compile_output}")
message(STATUS "std::string supports 3-way compare: " ${SEMVERUTIL_HAS_STRING_3_WAY_COMPARE})

file(
    WRITE
    ${CMAKE_CURRENT_BINARY_DIR}/support/string_view_iterator_pair_constructor.cpp
    "#include <string_view>

int main()
{
    char const kVal[] = \"Hello, World\";
    std::string_view a { kVal, kVal };
    return 0;
}"
)

try_compile(
    SEMVERUTIL_HAS_STRING_VIEW_ITERATOR_PAIR_CONSTRUCTOR
    ${CMAKE_CURRENT_BINARY_DIR}/support
    ${CMAKE_CURRENT_BINARY_DIR}/support/string_view_iterator_pair_constructor.cpp
    OUTPUT_VARIABLE compile_output
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)
message(DEBUG "${compile_output}")
message(STATUS "std::string_view supports iterator pair constructor: " ${SEMVERUTIL_HAS_STRING_VIEW_ITERATOR_PAIR_CONSTRUCTOR})

list(POP_BACK CMAKE_MESSAGE_INDENT)
