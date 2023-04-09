#include "cmdline.hpp"
#include "testing.hpp"
#include <cinttypes>
#include <span>
#include <string_view>

TEST_WITH_CONTEXT(should_parse_cmdline)
{
    std::span<char const*> cmdline { test_context.argv,
                                     std::size_t(test_context.argc) };
    auto opts = semver::parse_cmdline(cmdline);
    EXPECT(opts & semver::CmdLineSwitches::increment_major);
    EXPECT(opts & semver::CmdLineSwitches::increment_minor);
    EXPECT(opts & semver::CmdLineSwitches::increment_patch);
}

auto main(int argc, char const** argv) -> int
{
    return semver::testing::run_with_context(
        argc, argv, { TEST(should_parse_cmdline) });
}
