#include <cstdio>
#include <cassert>
#include <charconv>
#include <cctype>
#include <iostream>
#include <optional>
#include <string_view>

/*
std::optional: https://en.cppreference.com/w/cpp/utility/optional
std::from_chars: https://en.cppreference.com/w/cpp/utility/from_chars
std::string_view: https://en.cppreference.com/w/cpp/string/basic_string_view
std::nullopt: https://en.cppreference.com/w/cpp/utility/optional/nullopt
*/

std::optional<int> parse_int_strict(std::string_view str);

std::optional<int> parse_int_strict(std::string_view str)
{
    // Reject empty input.
    if (str.empty())
    {
        return std::nullopt;
    }

    // Reject whitespace anywhere in input.
    for (unsigned char c : str)
    {
        if (std::isspace(c))
        {
            return std::nullopt;
        }
    }

    int value{};
    const char *first = str.data();
    const char *last = first + str.size();

    const auto [ptr, ec] = std::from_chars(first, last, value, 10);
    if (ec != std::errc{})
    {
        // Includes invalid input and overflow/underflow (std::errc::result_out_of_range).
        return std::nullopt;
    }

    // All remaining characters must be digits:
    // `from_chars` stops at the first non-digit, so we require it to consume
    // the entire input (after optional sign) for strict parsing.
    if (ptr != last)
    {
        return std::nullopt;
    }
    return value;
}

int main()
{

    // test assertions

    // no blanks
    assert(parse_int_strict("") == std::nullopt);

    // 123 is as expected
    assert(parse_int_strict("123") == 123);

    // negative number
    assert(parse_int_strict("-7") == -7);

    // leading space
    assert(parse_int_strict(" 1") == std::nullopt);

    // trailing space
    assert(parse_int_strict("1 ") == std::nullopt);

    // embedded space
    assert(parse_int_strict("1 2") == std::nullopt);

    // non-digit characters
    assert(parse_int_strict("12x") == std::nullopt);

    // non-numeric characters
    assert(parse_int_strict("+") == std::nullopt);

    // overflow
    assert(parse_int_strict("2147483648") == std::nullopt);

    // underflow
    assert(parse_int_strict("-2147483649") == std::nullopt);

    std::cout << "All tests passed!\n";

    return 0;
}