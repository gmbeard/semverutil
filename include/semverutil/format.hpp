#ifndef SEMVERUTIL_FORMAT_HPP_INCLUDED
#define SEMVERUTIL_FORMAT_HPP_INCLUDED

#include "./semver.hpp"
#include <string>

namespace semver
{
auto write_formatted(SemVer const& val,
                     std::string_view format_string,
                     std::string& output) -> bool;
}

#endif // SEMVERUTIL_FORMAT_HPP_INCLUDED
