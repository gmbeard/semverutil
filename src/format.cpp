#include "semverutil/format.hpp"
#include "semverutil/detail/format.hpp"
#include <sstream>

auto semver::write_formatted(SemVer const& semver,
                             std::string_view format_string,
                             std::string& output) -> bool
{
    auto const handler = [&](std::string_view val, char token_type) {
        if (token_type == 0) {
            output += val;
            return true;
        }
        std::stringstream writer {};
        switch (token_type) {
        case 'M':
            writer << semver.major();
            break;
        case 'N':
            writer << semver.minor();
            break;
        case 'P':
            writer << semver.patch();
            break;
        case 'R':
            writer << semver.revision();
            break;
        case 'L':
            writer << semver.prerelease;
            break;
        case 'D':
            writer << semver.metadata;
            break;
        default:
            return false;
        }

        output += writer.str();

        return true;
    };

    return detail::parse_format_string(format_string, handler);
}
