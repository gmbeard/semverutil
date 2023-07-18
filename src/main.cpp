#include "semverutil/cmdline.hpp"
#include "semverutil/semverutil.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <span>
#include <vector>

auto operator<<(std::ostream& os, semver::SemVer const& val) -> std::ostream&
{
    os << val.version[0] << '.' << val.version[1] << '.' << val.version[2];
    if (val.version[3])
        os << '_' << val.version[3];

    if (val.prerelease.size())
        os << '-' << val.prerelease;

    if (val.metadata.size())
        os << '+' << val.metadata;

    return os;
}

auto main(int argc, char const** argv) -> int
{
    using semver::parse_cmdline;
    using semver::parse_multiple;
    using semver::SemVer;

    auto const [opts, args] =
        parse_cmdline(std::span<char const*> { argv, std::size_t(argc) });

    std::vector<char> buffer { std::istreambuf_iterator<char> { std::cin },
                               std::istreambuf_iterator<char> {} };
    std::vector<SemVer> results;

    parse_multiple(begin(buffer), end(buffer), results);

    if (!results.size())
        return 1;

    std::sort(rbegin(results), rend(results));
    auto r = std::move(results.front());

    if (opts & semver::CmdLineSwitches::increment_major) {
        r.version[0] += 1;
        r.version[1] = 0;
        r.version[2] = 0;
        r.version[3] = 0;
    }

    if (opts & semver::CmdLineSwitches::increment_minor) {
        r.version[1] += 1;
        r.version[2] = 0;
        r.version[3] = 0;
    }

    if (opts & semver::CmdLineSwitches::increment_patch) {
        r.version[2] += 1;
        r.version[3] = 0;
    }

    if (opts & semver::CmdLineSwitches::increment_revision) {
        r.version[3] += 1;
    }

    if (auto format_arg_pos =
            std::find_if(args.begin(),
                         args.end(),
                         [](auto arg) { return arg.size() && arg[0] == '+'; });
        format_arg_pos != args.end()) {
        std::string formatted_output;
        if (semver::write_formatted(
                r, format_arg_pos->substr(1), formatted_output))
            std::cout << formatted_output << '\n';
        else {
            std::cerr << "Invalid format string: " << format_arg_pos->substr(1)
                      << '\n';
            return 1;
        }
    }
    else {
        std::cout << r << '\n';
    }
    return 0;
}
