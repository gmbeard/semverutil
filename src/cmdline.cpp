#include "./cmdline.hpp"

namespace semver
{

auto parse_cmdline(std::span<char const*> cmdline) noexcept -> std::uint32_t
{
    std::uint32_t opts = 0;
    for (auto param : cmdline) {
        if (*param == '\0')
            break;

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
            }
        }
    }
    return opts;
}

} // namespace semver
