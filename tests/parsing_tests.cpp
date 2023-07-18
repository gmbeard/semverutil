#include "./testing.hpp"
#include "semverutil/semver.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using semver::parse_multiple;
using semver::parse_semver;
using semver::SemVer;

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

    for (auto const& r : results)
        std::cerr << r << '\n';
    std::cerr << results.size() << '\n';
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

auto should_parse_metadata_when_prerelease_omitted()
{
    constexpr char const kInput[] = "1.0.0+deadbeef";

    auto result = parse_semver(kInput);
    EXPECT(result);
    EXPECT(result->prerelease.size() == 0);
    EXPECT(result->metadata == "deadbeef");
}

auto should_handle_multiple_prerelease_separators_and_metadata()
{
    constexpr char const kInput[] = "1.0.0-a-b+c+d";

    auto result = parse_semver(kInput);
    EXPECT(result);
    EXPECT(result->prerelease == "a-b");
    EXPECT(result->metadata == "c+d");
}

auto main() -> int
{
    return semver::testing::run(
        { TEST(should_parse_multiple),
          TEST(should_parse_from_stdin),
          TEST(should_parse_revision_when_incomplete_core),
          TEST(should_handle_multiple_prerelease_separators_and_metadata),
          TEST(should_parse_metadata_when_prerelease_omitted) });
}
