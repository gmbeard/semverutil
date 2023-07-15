#include "testing.hpp"
#include <array>
#include <cinttypes>
#include <iostream>
#include <limits>
#include <source_location>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

struct Contract
{
    using Failure = auto (*)(char const* msg, std::source_location) -> void;

    explicit Contract(Failure failure) noexcept
        : failure_ { failure }
    {
    }

    auto
    expects(bool condition,
            char const* msg = "",
            std::source_location loc = std::source_location::current()) const
        -> void
    {
        if (!condition)
            failure_(msg, loc);
    }

    auto
    ensures(bool condition,
            char const* msg = "",
            std::source_location loc = std::source_location::current()) const
        -> void
    {
        expects(condition, msg, loc);
    }

private:
    Failure failure_;
};

[[noreturn]] auto inline report_and_throw(char const* msg,
                                          std::source_location loc) -> void
{
    std::cerr << msg << " failure: " << loc.function_name() << " "
              << loc.file_name() << ':' << loc.line() << '\n';

    throw std::logic_error { msg };
}

[[noreturn]] auto inline report_and_terminate(char const* msg,
                                              std::source_location loc) -> void
{
    std::cerr << msg << " failure: " << loc.function_name() << " "
              << loc.file_name() << ':' << loc.line() << '\n';

    std::terminate();
}

auto inline const contract = Contract(report_and_throw);

template <typename To, typename From>
[[nodiscard]] auto
safe_cast(From&& from,
          std::source_location loc = std::source_location::current()) -> To
requires(std::is_integral_v<std::remove_cvref_t<From>> ==
         std::is_integral_v<To>)
{
    auto constexpr kSafeCast = "safe_cast";
    auto val = static_cast<To>(from);
    if constexpr (std::is_signed_v<std::remove_cvref_t<From>> ==
                  std::is_signed_v<To>) {
        contract.ensures(static_cast<std::remove_cvref_t<From>>(val) ==
                             std::forward<From>(from),
                         kSafeCast,
                         loc);
    }
    else {
        contract.ensures(static_cast<std::remove_cvref_t<From>>(val) ==
                                 std::forward<From>(from) &&
                             (val < To {}) == (std::forward<From>(from) <
                                               std::remove_cvref_t<From> {}),
                         kSafeCast,
                         loc);
    }
    return val;
}

template <typename Source, typename Index>
[[nodiscard]] auto
safe_subscript(Source&& src,
               Index&& idx,
               std::source_location loc = std::source_location::current())
    -> decltype(auto)
requires(std::is_integral_v<std::remove_cvref_t<Index>> && requires {
    std::size(src);
    src[idx];
    std::begin(src) + 2;
})
{
    auto constexpr kCheckBounds = "check_bounds";
    auto idx_ = safe_cast<decltype(std::size(std::forward<Source>(src)))>(
        std::forward<Index>(idx), loc);
    contract.expects(0 <= idx && idx_ < std::size(std::forward<Source>(src)),
                     kCheckBounds,
                     loc);
    return std::forward<Source>(src)[idx_];
}

auto should_fail_safe_subscript()
{
    std::vector<int> v { 1, 2, 3 };

    EXPECT_THROWS(static_cast<void>(safe_subscript(v, 3)));
}

auto should_fail_safe_cast()
{
    EXPECT_THROWS(static_cast<void>(
        safe_cast<std::int16_t>(std::numeric_limits<std::int32_t>::max())));
}

auto main() -> int
{
    return semver::testing::run({
        TEST(should_fail_safe_subscript),
        TEST(should_fail_safe_cast),
    });
}
