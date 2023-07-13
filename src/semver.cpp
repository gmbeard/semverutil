#include "semver.hpp"

#include <iostream>
#ifndef SEMVERUTIL_HAS_FULL_3_WAY_COMPARE
#include <tuple>
#endif

namespace semver
{

#ifdef SEMVERUTIL_HAS_FULL_3_WAY_COMPARE
auto SemVer::operator<=>(SemVer const& rhs) const noexcept
    -> std::partial_ordering
{
    if (auto cmp = version <=> rhs.version; cmp != 0) {
        return cmp;
    }

    if (prerelease.size() == 0 && rhs.prerelease.size() > 0)
        return std::partial_ordering::greater;
    else if (prerelease.size() > 0 && rhs.prerelease.size() == 0)
        return std::partial_ordering::less;

    return prerelease <=> rhs.prerelease;
}
#else
auto SemVer::operator<(SemVer const& rhs) const noexcept -> bool
{
    return std::tie(version, prerelease) <
           std::tie(rhs.version, rhs.prerelease);
}

auto operator!=(SemVer const& lhs, SemVer const& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

auto operator<=(SemVer const& lhs, SemVer const& rhs) noexcept -> bool
{
    return (lhs < rhs) || (lhs == rhs);
}

auto operator>(SemVer const& lhs, SemVer const& rhs) noexcept -> bool
{
    return !(lhs <= rhs);
}

auto operator>=(SemVer const& lhs, SemVer const& rhs) noexcept -> bool
{
    return (lhs > rhs) || (lhs == rhs);
}
#endif

auto SemVer::operator==(SemVer const& rhs) const noexcept -> bool
{
    return version == rhs.version && prerelease == rhs.prerelease;
}

auto parse_semver(char const* str) -> decltype(parse_semver(str, str))
{
    return parse_semver(str, str + std::strlen(str));
}

} // namespace semver
