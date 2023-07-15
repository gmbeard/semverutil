#include "./testing.hpp"
#include "semverutil/config.hpp"
#include "semverutil/detail/semver.hpp"
#include "semverutil/utils.hpp"
#include <array>
#include <cstring>
#include <iostream>
#include <iterator>
#include <span>
#include <string_view>
#include <vector>

auto read_line(std::string_view data, std::string_view& line)
    -> std::pair<int, std::string_view>
{
    auto start_pos = data.begin();
    auto split_pos = data.begin();

    enum
    {
        CHAR,
        LF,
        NL,

        EXTRACT,
        END,

        ERROR
    } state = CHAR;

    int skip = 0;

    while (state < END && split_pos != data.end()) {
        switch (state) {
        case CHAR:
            switch (*split_pos) {
            case '\r':
                state = LF;
                break;
            case '\n':
                state = NL;
                break;
            default:
                ++split_pos;
                if (split_pos == data.end())
                    state = EXTRACT;
                break;
            }
            break;
        case LF:
            if (*split_pos != '\r')
                state = ERROR;
            else {
                ++skip;
                ++split_pos;
                state = NL;
            }
            break;
        case NL:
            if (*(split_pos + skip) != '\n')
                state = ERROR;
            else {
                ++skip;
                state = EXTRACT;
            }
            break;
        case EXTRACT:
#ifdef SEMVERUTIL_HAS_STRING_VIEW_ITERATOR_PAIR_CONSTRUCTOR
            line = { start_pos, split_pos };
#else
            line = data.substr(std::size_t(start_pos - data.begin()),
                               std::size_t(split_pos - start_pos));
#endif
            if (split_pos != data.end())
                split_pos += skip;

            start_pos = split_pos;
            skip = 0;
            state = END;
            break;
        default:
            break;
        }
    }

#ifdef SEMVERUTIL_HAS_STRING_VIEW_ITERATOR_PAIR_CONSTRUCTOR
    return { state == END ? 0 : state, { start_pos, data.end() } };
#else
    return { state == END ? 0 : state,
             data.substr(std::size_t(start_pos - data.begin())) };
#endif
}

auto should_be_valid_semver(std::string_view value, bool check_valid) -> void
{
    std::array<std::size_t, 4> storage;

    auto const result = semver::detail::parse_version(
        value.data(), value.size(), begin(storage), end(storage));

    if ((result.error == 0) != check_valid) {
        std::cerr << "Expected semver to be "
                  << (check_valid ? "valid" : "invalid") << ": " << value
                  << '\n';
    }

    EXPECT((result.error == 0) == check_valid);
}

TEST_WITH_CONTEXT(should_be_valid_semver_input)
{
    using semver::testing::TestFailure;

    std::span<char const*> args = { test_context.argv,
                                    std::size_t(test_context.argc) };

    auto test_for_invalid =
        std::find_if(args.begin(), args.end(), [](auto const& arg) {
            return std::strcmp(arg, "-N") == 0 ||
                   std::strcmp(arg, "--invalid") == 0;
        }) != args.end();

    std::vector<char> buffer;
    std::copy(std::istreambuf_iterator<char> { std::cin },
              std::istreambuf_iterator<char> {},
              std::back_inserter(buffer));

    std::string_view remaining_data { buffer.data(), buffer.size() };
    std::size_t n = 0;
    std::size_t error_count = 0;
    while (remaining_data.size()) {
        std::string_view line;
        auto const [error, rest] = read_line(remaining_data, line);
        if (error) {
            std::cerr << "Couldn't read line: " << (n + 1) << '\n';
        }
        EXPECT(!error);
        try {
            should_be_valid_semver(line, !test_for_invalid);
        }
        catch (TestFailure const&) {
            ++error_count;
        }
        catch (...) {
            throw;
        }
        remaining_data = rest;
    }

    EXPECT(error_count == 0);
}

TEST_WITH_CONTEXT(should_parse_version_using_state)
{
    constexpr char const kInput[] = "0.12.345_6789";
    std::array<std::size_t, 4> result;

    auto const [err, output_end, input_end] = semver::detail::parse_version(
        kInput, std::strlen(kInput), begin(result), end(result));

    std::cerr << (output_end - begin(result)) << '\n';
    std::cerr << err << '\n';

    EXPECT(!err);
    EXPECT(output_end == end(result));
    EXPECT(result[0] == 0);
    EXPECT(result[1] == 12);
    EXPECT(result[2] == 345);
    EXPECT(result[3] == 6789);
}

auto main(int argc, char const** argv) -> int
{
    return semver::testing::run_with_context(
        argc,
        argv,
        { TEST(should_parse_version_using_state),
          TEST(should_be_valid_semver_input) });
}
