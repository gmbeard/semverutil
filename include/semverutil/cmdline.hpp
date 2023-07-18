#ifndef SEMVER_CMDLINE_HPP_INCLUDED
#define SEMVER_CMDLINE_HPP_INCLUDED

#include <cinttypes>
#include <span>
#include <string_view>
#include <vector>

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
    increment_revision = 8,
};

} // namespace CmdLineSwitches

struct ParseCmdLineResult
{
    std::uint32_t options;
    std::vector<std::string_view> args;
};

auto parse_cmdline(std::span<char const*> cmdline) noexcept
    -> ParseCmdLineResult;

} // namespace semver

#endif // SEMVER_CMDLINE_HPP_INCLUDED
