#ifndef SEMVERUTIL_UTILS_HPP_INCLUDED
#define SEMVERUTIL_UTILS_HPP_INCLUDED

#include <algorithm>
#include <cstddef>
#include <type_traits>

namespace semver
{

struct AnyOf;
auto operator==(char, AnyOf const&) noexcept -> bool;

struct AnyOf
{
    AnyOf() noexcept;
    explicit AnyOf(char const* chars) noexcept;

    friend auto operator==(char, AnyOf const&) noexcept -> bool;

private:
    char const* chars_;
};

// clang-format off
template <typename T, typename It>
concept Transform = requires(T f, It i) {
    { f(i, i) };
};

template <typename T, typename It>
concept TransformWithIndex = requires(T f, It i, std::size_t n) {
    { f(i, i, n) };
};
// clang-format on

template <typename T, typename It>
concept EitherTransform = Transform<T, It> || TransformWithIndex<T, It>;

template <typename InputIterator,
          typename Delim,
          EitherTransform<InputIterator> F,
          typename OutputIterator>
auto split_to(InputIterator i_first,
              InputIterator i_last,
              Delim const& delim,
              OutputIterator o_first,
              OutputIterator o_last,
              F transform) noexcept -> OutputIterator
{
    if (i_first == i_last)
        return o_first;

    auto from = i_first;
    auto delim_pos = std::find(from, i_last, delim);
    std::size_t n = 0;

    while (o_first != o_last) {
        if (std::next(o_first) == o_last)
            delim_pos = i_last;
        if constexpr (Transform<F, InputIterator>) {
            *o_first++ = transform(from, delim_pos);
        }
        else {
            *o_first++ = transform(from, delim_pos, n++);
        }

        from = delim_pos != i_last ? ++delim_pos : delim_pos;
        delim_pos = std::find(from, i_last, delim);

        if (delim_pos == i_last && from == delim_pos)
            break;
    }

    return o_first;
}

template <typename InputIterator, typename T, typename OutputIterator>
auto split_to(InputIterator i_first,
              InputIterator i_last,
              T const& delim,
              OutputIterator o_first,
              OutputIterator o_last) noexcept -> OutputIterator
{
    using OutputType = std::decay_t<decltype(*o_first)>;
    return split_to(
        i_first, i_last, delim, o_first, o_last, [&](auto u, auto v) {
#ifdef SEMVERUTIL_HAS_STRING_VIEW_ITERATOR_PAIR_CONSTRUCTOR
            return OutputType(u, v);
#else
            return OutputType(
                    &*i_first + static_cast<std::size_t>(u - i_first),
                    static_cast<std::size_t>(v - u));
#endif
        });
}

template <typename InputIterator, typename T>
auto parse_numeric_value(InputIterator first,
                         InputIterator last,
                         T& val) noexcept -> bool
{
    val = T {};
    for (; first != last; ++first) {
        if (*first < '0' || *first > '9')
            return false;

        val = val * 10 + static_cast<T>(*first - '0');
    }

    return true;
}

} // namespace semver

#endif // SEMVERUTIL_UTILS_HPP_INCLUDED
