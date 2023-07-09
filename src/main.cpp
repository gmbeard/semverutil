#include "./cmdline.hpp"
#include "./semver.hpp"
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

    auto opts =
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

    std::cout << r << '\n';
    return 0;
}
