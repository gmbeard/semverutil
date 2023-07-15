#include "testing.hpp"
#include <algorithm>
#include <array>
#include <cinttypes>
#include <cstring>
#include <iostream>
#include <span>
#include <string_view>

namespace options
{

enum : std::uint32_t
{
    PrereleaseTag = 1,
    IncrementMajor = 2,
    IncrementMinor = 3,
    IncrementPatch = 4,
    IncrementPrerelease = 5,
};

}

namespace flags
{

enum : std::uint32_t
{
    UsesArgument = 1,
    ArgumentOptional = 2,
};

}

struct Option
{
    std::uint32_t type;
    std::uint32_t flags;
};

struct Argument
{
    Option option;
    std::string_view value;
};

auto identify_option(char val) noexcept -> Option
{
    switch (val) {
    case 'T':
        return { options::PrereleaseTag, flags::UsesArgument };
    case 'M':
        return { options::IncrementMajor, 0 };
    case 'N':
        return { options::IncrementMinor, 0 };
    case 'P':
        return { options::IncrementPatch, 0 };
    default:
        return { 0, 0 };
    }
}

template <typename It, typename F>
auto eval_arguments(It first, It last, F f)
{
    auto end_opts = std::find_if(
        first, last, [](auto arg) { return std::strcmp(arg, "--") == 0; });

    auto end = end_opts;

    auto i = first;
    while (i != end_opts) {
        auto curr = i;
        std::string_view arg { *curr };
        if (arg.front() != '-' || arg.size() == 1) {
            std::rotate(curr, std::next(i), end);
            end_opts--;

            continue;
        }

        ++i;

        arg = arg.substr(1);

        auto const [type, flags] = identify_option(arg.front());
        if (!(flags & flags::UsesArgument)) {
            if (type)
                f(Argument { { type, flags }, std::string_view {} });

            continue;
        }

        arg = arg.substr(1);
        if (arg.size()) {
            f(Argument { { type, flags }, arg });
            continue;
        }

        if (i != end_opts) {
            std::string_view param { *i++ };

            if ((flags & flags::ArgumentOptional) && param.front() == '-' &&
                param.size() > 1) {
                f(Argument { { type, flags }, {} });
            }
            else {
                f(Argument { { type, flags }, param });
            }
        }
    }

    return end_opts;
}

auto basic_parse() -> void
{
    char const* args[] = { "-T", "b", "-", "c", "d", "-N", "-P", "--", "val" };

    auto i = eval_arguments(std::begin(args), std::end(args), [](auto arg) {
        std::cerr << arg.option.type << ',' << arg.option.flags << ','
                  << arg.value << '\n';
    });

    std::cerr << '\n';
    for (; i != std::end(args); ++i) {
        std::cerr << *i << '\n';
    }

    EXPECT(false);
}

auto main() -> int
{
    //
    return semver::testing::run({ TEST(basic_parse) });
}
