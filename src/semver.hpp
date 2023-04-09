#ifndef SEMVERUTIL_SEMVER_HPP_INCLUDED
#define SEMVERUTIL_SEMVER_HPP_INCLUDED

#include "./utils.hpp"
#include <array>
#include <cstring>
#include <optional>
#include <string>
#include <utility>

namespace semver
{
struct SemVer
{
    std::array<uint32_t, 3> version;
    std::string prerelease;
    std::string metadata;

    auto operator<=>(SemVer const& rhs) const noexcept -> std::partial_ordering;
    auto operator==(SemVer const& rhs) const noexcept -> bool;
};

template <typename InputIterator>
auto parse_semver(InputIterator first, InputIterator last)
    -> std::optional<SemVer>
{
    std::array<std::string_view, 2> core_and_remaining {};
    auto r = split_to(
        first, last, '-', begin(core_and_remaining), end(core_and_remaining));

    if (r == begin(core_and_remaining))
        return std::nullopt;

    decltype(SemVer::version) version {};

    auto const version_split_end =
        split_to(begin(core_and_remaining[0]),
                 end(core_and_remaining[0]),
                 '.',
                 version.begin(),
                 version.end(),
                 [](auto u, auto v) {
                     std::remove_reference_t<decltype(version[0])> val;
                     parse_numeric_value(u, v, val);
                     return val;
                 });

    if (version_split_end != end(version))
        return std::nullopt;

    std::array<std::string_view, 2> prerelease_and_metadata {};

    if (r == end(core_and_remaining)) {
        r = split_to(begin(core_and_remaining[1]),
                     end(core_and_remaining[1]),
                     '+',
                     begin(prerelease_and_metadata),
                     end(prerelease_and_metadata));
    }

    return SemVer { version,
                    std::string { prerelease_and_metadata[0] },
                    std::string { prerelease_and_metadata[1] } };
}

template <typename InputIterator, typename Container>
auto parse_multiple(InputIterator first,
                    InputIterator last,
                    Container& container)
    -> decltype(std::begin(std::as_const(container)))
{
    std::array<std::string_view, 2> splits {};
    AnyOf const delims { " \t\r\n" };
    auto split_pos = split_to(first, last, delims, begin(splits), end(splits));

    while (split_pos == end(splits)) {
        auto semver = parse_semver(begin(splits[0]), end(splits[0]));
        if (semver)
            container.push_back(std::move(*semver));

        split_pos = split_to(begin(splits[1]),
                             end(splits[1]),
                             delims,
                             begin(splits),
                             end(splits));
    }

    if (split_pos != begin(splits)) {
        if (auto semver = parse_semver(begin(splits[0]), end(splits[0]));
            semver) {
            container.push_back(std::move(*semver));
        }
    }

    return std::end(container);
}

auto parse_semver(char const* str) -> decltype(parse_semver(str, str));

} // namespace semver

#endif // SEMVERUTIL_SEMVER_HPP_INCLUDED
