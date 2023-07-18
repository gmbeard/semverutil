#ifndef SEMVERUTIL_DETAIL_FORMAT_HPP_INCLUDED
#define SEMVERUTIL_DETAIL_FORMAT_HPP_INCLUDED

#include <cstddef>
#include <string_view>

namespace semver::detail
{

template <typename UnaryFunction>
auto parse_format_string(std::string_view input, UnaryFunction f) -> bool
{
    enum
    {
        Scanning,
        VariablePrefix,
        Variable,
    } state = Scanning;

    auto p = input.begin();
    auto token_start = p;
    auto data_end = input.end();
    unsigned prefixed = 0;

    while (p != data_end) {
        switch (state) {
        case Scanning:
            if (*p == '%')
                state = VariablePrefix;
            else
                ++p;
            break;
        case VariablePrefix:
            prefixed = 1 - prefixed;
            state = prefixed ? Variable : Scanning;
            if (!f({ token_start, std::size_t(p - token_start) },
                   static_cast<char>(0)))
                return false;
            token_start = ++p;
            break;
        case Variable:
            ++p;
            if (!f({ token_start, std::size_t(p - token_start) }, *token_start))
                return false;
            token_start = p;
            state = Scanning;
            prefixed = 0;
            break;
        }
    }

    if (!f({ token_start, std::size_t(p - token_start) }, static_cast<char>(0)))
        return false;

    return state == Scanning;
}
} // namespace semver::detail
#endif // SEMVERUTIL_DETAIL_FORMAT_HPP_INCLUDED
