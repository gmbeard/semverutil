#include "semverutil/cmdline.hpp"

namespace semver
{

auto parse_cmdline(std::span<char const*> cmdline) noexcept
    -> ParseCmdLineResult
{
    std::uint32_t opts = 0;
    std::vector<std::string_view> args;
    for (auto param : cmdline) {
        if (*param == '\0')
            break;

        if (*param == '+') {
            args.push_back(param);
            continue;
        }

        if (*param++ != '-')
            continue;

        for (; *param != '\0'; ++param) {
            switch (*param) {
            case 'M':
                opts |= CmdLineSwitches::increment_major;
                break;
            case 'N':
                opts |= CmdLineSwitches::increment_minor;
                break;
            case 'P':
                opts |= CmdLineSwitches::increment_patch;
                break;
            case 'R':
                opts |= CmdLineSwitches::increment_revision;
                break;
            }
        }
    }
    return { opts, std::move(args) };
}

} // namespace semver
