#include "./semver.hpp"

namespace semver::detail
{

auto is_digit(char val) noexcept -> bool
{
    return (val - '0') >= 0 && (val - '0') <= 9;
}

} // namespace semver::detail
