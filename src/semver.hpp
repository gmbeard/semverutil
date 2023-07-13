#ifndef SEMVERUTIL_SEMVER_HPP_INCLUDED
#define SEMVERUTIL_SEMVER_HPP_INCLUDED

#include "./detail/semver.hpp"
#include "./utils.hpp"
#include "semverutil/config.hpp"
#include <array>
#include <cstring>
#include <optional>
#include <string>
#include <utility>
#ifdef SEMVERUTIL_HAS_FULL_3_WAY_COMPARE
#include <compare>
#endif

namespace semver
{
struct SemVer
{
    std::array<uint32_t, 4> version;
    std::string prerelease;
    std::string metadata;

    auto operator==(SemVer const& rhs) const noexcept -> bool;
#ifdef SEMVERUTIL_HAS_FULL_3_WAY_COMPARE
    auto operator<=>(SemVer const& rhs) const noexcept -> std::partial_ordering;
#else
    auto operator<(SemVer const& rhs) const noexcept -> bool;
#endif

    auto major() const noexcept { return version[0]; };
    auto minor() const noexcept { return version[1]; };
    auto patch() const noexcept { return version[2]; };
    auto revision() const noexcept { return version[3]; };
};

#ifndef SEMVERUTIL_HAS_FULL_3_WAY_COMPARE
auto operator!=(SemVer const& lhs, SemVer const& rhs) noexcept -> bool;
auto operator<=(SemVer const& lhs, SemVer const& rhs) noexcept -> bool;
auto operator>(SemVer const& lhs, SemVer const& rhs) noexcept -> bool;
auto operator>=(SemVer const& lhs, SemVer const& rhs) noexcept -> bool;
#endif

template <typename InputIterator>
auto parse_semver(InputIterator first, InputIterator last)
    -> std::optional<SemVer>
{
    std::string_view const empty = "";

    std::array<std::string_view, 3> parts {};
    auto r = split_to(first, last, AnyOf { "-+" }, begin(parts), end(parts));

    if (r == begin(parts))
        return std::nullopt;

    while (r != end(parts))
        *r++ = empty;

    decltype(SemVer::version) version {};

    auto [error, out_pos, in_pos] = detail::parse_version(
        parts[0].data(), parts[0].size(), version.begin(), version.end());

    if (error)
        return std::nullopt;

    while (out_pos != version.end())
        *out_pos++ = 0;

    return SemVer { version,
                    std::string { parts[1] },
                    std::string { parts[2] } };
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
