#include "./utils.hpp"

namespace semver
{
AnyOf::AnyOf() noexcept
    : chars_ { nullptr }
{
}

AnyOf::AnyOf(char const* chars) noexcept
    : chars_ { chars }
{
}

} // namespace semver

auto semver::operator==(char lhs, semver::AnyOf const& rhs) noexcept -> bool
{
    if (!rhs.chars_)
        return false;

    char const* p = rhs.chars_;
    while (*p) {
        if (lhs == *p++)
            return true;
    }

    return false;
}
