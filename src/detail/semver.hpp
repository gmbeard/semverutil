#ifndef SEMVERUTIL_SEMVER_DETAIL_SEMVER_HPP_INCLUDED
#define SEMVERUTIL_SEMVER_DETAIL_SEMVER_HPP_INCLUDED

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace semver::detail
{
auto is_digit(char val) noexcept -> bool;

template <typename T>
auto convert_to(char const* data, std::size_t len) -> T
requires(std::is_unsigned_v<T>)
{
    T result {};
    while (len--) {
        char c = *data++;
        result = result * 10 + static_cast<T>(c - '0');
    }

    return result;
}

template <typename OutputIterator>
struct ParseResult
{
    int error;
    OutputIterator output_end;
    char const* input_end;
};

template <typename OutputIterator>
auto parse_version(char const* data,
                   std::size_t data_len,
                   OutputIterator o_pos,
                   OutputIterator o_last) noexcept
    -> ParseResult<OutputIterator>
{
    if (!data_len)
        return { true, o_pos, data };

    enum
    {
        FIRST_DIGIT = 1,
        DIGIT,
        DELIMITER,
        CONSUME_DIGITS,

        END,

        /* Errors go below here...
         */
        ERROR_UNEXPECTED_TOKEN,
        ERROR_UNEXPECTED_EOF,
    } state = FIRST_DIGIT;

    char const* const data_end = data + data_len;
    char const* value_end = data;
    unsigned part_index = 0;

    while (state < END && o_pos != o_last) {
        switch (state) {
        case FIRST_DIGIT:
            if (!is_digit(*value_end)) {
                state = ERROR_UNEXPECTED_TOKEN;
            }
            else {
                state = (*value_end == '0') ? DELIMITER : DIGIT;
                ++value_end;
                state = (value_end == data_end) ? CONSUME_DIGITS : state;
            }
            break;

        case DIGIT:
            if (!is_digit(*value_end)) {
                state = DELIMITER;
            }
            else {
                ++value_end;
                state = (value_end == data_end) ? CONSUME_DIGITS : state;
            }
            break;

        case DELIMITER:
            switch (*value_end) {
            case '.':
                state =
                    part_index < 2 ? CONSUME_DIGITS : ERROR_UNEXPECTED_TOKEN;
                break;
            case '_':
                state =
                    part_index == 2 ? CONSUME_DIGITS : ERROR_UNEXPECTED_TOKEN;
                break;
            default:
                state = ERROR_UNEXPECTED_TOKEN;
                break;
            }
            break;

        case CONSUME_DIGITS:
            *o_pos++ = convert_to<
                typename std::iterator_traits<OutputIterator>::value_type>(
                data, std::size_t(value_end - data));
            if (value_end != data_end) {
                data = ++value_end;
                state = FIRST_DIGIT;
            }
            else {
                state = END;
            }
            ++part_index;
            break;

        default:
            break;
        }

        if (value_end == data_end && state < CONSUME_DIGITS) {
            /* Looks like the input ended on a delimiter
             */
            state = ERROR_UNEXPECTED_EOF;
        }
    }

    return { state == END || o_pos == o_last ? 0 : state, o_pos, value_end };
}

} // namespace semver::detail

#endif // SEMVERUTIL_SEMVER_DETAIL_SEMVER_HPP_INCLUDED
