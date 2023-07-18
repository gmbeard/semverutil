#include "semverutil/detail/format.hpp"
#include "testing.hpp"
#include <iostream>

auto should_parse_format_string()
{
    char const* kInput = "%%%M.%N.%P_%R-%L-%T";
    auto success = semver::detail::parse_format_string(
        kInput, [](std::string_view token, char chr) {
            std::cerr << "- \"" << token << "\" " << (chr ? chr : '0') << '\n';
            return true;
        });

    EXPECT(success);
}

auto main() -> int
{
    return semver::testing::run({ TEST(should_parse_format_string) });
}
