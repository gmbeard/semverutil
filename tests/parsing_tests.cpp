#include "./testing.hpp"
#include "semver.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using semver::parse_multiple;
using semver::parse_semver;
using semver::SemVer;

auto should_parse_multiple() -> void
{
    constexpr char const kInput[] = R"#(1.0.1
1.0.2+deadbeef
1.0.3
1.0.4-prerelease.1     1.0.4 
1.23.456_78910-nightly.1+deadbeef
")#";

    std::vector<SemVer> results;

    parse_multiple(std::begin(kInput), std::end(kInput), results);

    EXPECT(results.size() == 6);

    std::sort(begin(results), end(results));
    auto comp = parse_semver("1.23.456_78910-nightly.1");

    EXPECT(comp);
    EXPECT(*comp == results.back());
}

auto should_parse_from_stdin() -> void
{
    std::vector<char> buffer { std::istreambuf_iterator<char> { std::cin },
                               std::istreambuf_iterator<char> {} };

    std::vector<SemVer> results;
    parse_multiple(begin(buffer), end(buffer), results);

    EXPECT(results.size());
}

auto should_parse_revision_when_incomplete_core()
{
    constexpr char const kInput[] = R"#(1_1
1.2_2
")#";

    std::vector<SemVer> results;
    parse_multiple(std::begin(kInput), std::end(kInput), results);
    EXPECT(results.size() == 2);

    EXPECT(results[0].major() == 1);
    EXPECT(results[0].minor() == 0);
    EXPECT(results[0].patch() == 0);
    EXPECT(results[0].revision() == 1);

    EXPECT(results[1].major() == 1);
    EXPECT(results[1].minor() == 2);
    EXPECT(results[1].patch() == 0);
    EXPECT(results[1].revision() == 2);
}

auto main() -> int
{
    return semver::testing::run(
        { TEST(should_parse_multiple),
          TEST(should_parse_from_stdin),
          TEST(should_parse_revision_when_incomplete_core) });
}
