#ifndef SEMVER_CMDLINE_HPP_INCLUDED
#define SEMVER_CMDLINE_HPP_INCLUDED

#include <cinttypes>
#include <span>

namespace semver
{

namespace CmdLineSwitches
{

enum : std::uint32_t
{
    empty = 0,
    increment_major = 1,
    increment_minor = 2,
    increment_patch = 4,
};

}

auto parse_cmdline(std::span<char const*> cmdline) noexcept -> std::uint32_t;

} // namespace semver

#endif // SEMVER_CMDLINE_HPP_INCLUDED
