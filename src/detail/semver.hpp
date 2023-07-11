#ifndef SEMVERUTIL_SEMVER_DETAIL_SEMVER_HPP_INCLUDED
#define SEMVERUTIL_SEMVER_DETAIL_SEMVER_HPP_INCLUDED

#include <cstddef>
#include <iterator>
#include <limits>
#include <type_traits>

namespace semver::detail
{
auto is_digit(char val) noexcept -> bool;

template <typename T>
auto convert_to(char const* data, std::size_t len, T& result) -> bool
requires(std::is_unsigned_v<T>)
{
    result = T(0);
    while (len--) {
        char c = *data++;
        /* Check for overflow...
         */
        if ((std::numeric_limits<T>::max() / 10 - static_cast<T>(c - '0')) <
            result)
            return false;
        result = result * 10 + static_cast<T>(c - '0');
    }

    return true;
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
        FIRST_REVISION_DIGIT,
        REVISION_DIGIT,
        DELIMITER,
        CONSUME_DIGITS,

        END,

        /* Errors go below here...
         */
        ERROR_UNEXPECTED_TOKEN,
        ERROR_UNEXPECTED_EOF,
        ERROR_OVERFLOW,
    } state = FIRST_DIGIT;

    char const* const data_end = data + data_len;
    char const* value_end = data;
    unsigned part_index = 0;
    bool got_early_revision_value = false;

    while (state < END && o_pos != o_last) {
        switch (state) {
        case FIRST_DIGIT:
            if (!is_digit(*value_end)) {
                state = ERROR_UNEXPECTED_TOKEN;
                break;
            }

            state = (*value_end == '0') ? DELIMITER : DIGIT;
            ++value_end;
            state = (value_end == data_end) ? CONSUME_DIGITS : state;
            break;

        case DIGIT:
            if (!is_digit(*value_end)) {
                state = DELIMITER;
                break;
            }

            ++value_end;
            state = (value_end == data_end) ? CONSUME_DIGITS : state;
            break;

        case DELIMITER:
            switch (*value_end) {
            case '.':
                state =
                    part_index < 2 ? CONSUME_DIGITS : ERROR_UNEXPECTED_TOKEN;
                break;
            case '_':
                state = CONSUME_DIGITS;
                break;
            default:
                state = ERROR_UNEXPECTED_TOKEN;
                break;
            }
            break;

        case CONSUME_DIGITS:
            /* If we encountered the revision value early then
             * we must set all but the last remaining output values
             * to zero...
             */
            if (got_early_revision_value)
                while (std::next(o_pos) != o_last)
                    *o_pos++ = 0;

            if (!convert_to(data, std::size_t(value_end - data), *o_pos)) {
                state = ERROR_OVERFLOW;
                break;
            }

            ++o_pos;
            ++part_index;

            if (value_end == data_end) {
                state = END;
                break;
            }

            /* Have we encountered an early revision value?...
             */
            got_early_revision_value = *value_end == '_';
            state = FIRST_DIGIT;
            data = ++value_end;
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

    if (value_end != data_end)
        return { ERROR_UNEXPECTED_TOKEN, o_pos, value_end };

    return { state == END || o_pos == o_last ? 0 : state, o_pos, value_end };
}

} // namespace semver::detail

#endif // SEMVERUTIL_SEMVER_DETAIL_SEMVER_HPP_INCLUDED
