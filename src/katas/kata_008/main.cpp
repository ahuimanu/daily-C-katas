#include <cassert>
#include <cctype>
#include <charconv>
#include <expected>
#include <string_view>

/*
Task

Replace boolean/error-code returns with std::expected to propagate rich failure information.

Requirements

Single file main.cpp.

Define:

enum class ParseErr {
  empty,
  bad_format,
  out_of_range
};


Implement:

std::expected<int, ParseErr> parse_percent(std::string_view s);


Rules:

Accept only "NN%" where:
NN is 1–3 digits
Value must be 0–100 inclusive
% must be present and last

Reject:

empty string → empty
missing %, non-digits → bad_format
numeric value outside 0..100 → out_of_range
No exceptions.

In main() use assert to verify:

Valid:

"0%" → 0
"5%" → 5
"100%" → 100

Errors:

"" → empty
"10" → bad_format
"10 %" → bad_format
"abc%" → bad_format
"101%" → out_of_range

Constraints

C++23 only
Use std::expected
No exceptions
No logging

*/

enum class ParseErr
{
    empty,
    bad_format,
    out_of_range
};

std::expected<int, ParseErr> parse_percent(std::string_view s);

std::expected<int, ParseErr> parse_percent(std::string_view s)
{
    // Check for empty string
    if (s.empty())
    {
        return std::unexpected(ParseErr::empty);
    }
    // Check for '%' at the end
    if (s.back() != '%')
    {
        return std::unexpected(ParseErr::bad_format);
    }

    // Extract the numeric part
    std::string_view number_part = s.substr(0, s.size() - 1);

    // Validate the numeric part
    if (number_part.empty() || number_part.size() > 3)
    {
        return std::unexpected(ParseErr::bad_format);
    }

    // Check if all characters are digits
    for (char c : number_part)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            return std::unexpected(ParseErr::bad_format);
        }
    }

    // Convert to integer
    int value = 0;

    auto [ptr, ec] = std::from_chars(number_part.data(), number_part.data() + number_part.size(), value, 10);
    if (ec != std::errc() || ptr != number_part.data() + number_part.size())
    {
        return std::unexpected(ParseErr::bad_format);
    }

    // Check range
    if (value < 0 || value > 100)
    {
        return std::unexpected(ParseErr::out_of_range);
    }
    return value;
}

int main()
{
    // Valid cases
    assert(parse_percent("0%").value() == 0);
    assert(parse_percent("5%").value() == 5);
    assert(parse_percent("100%").value() == 100);

    // Error cases
    assert(parse_percent("").error() == ParseErr::empty);
    assert(parse_percent("10").error() == ParseErr::bad_format);
    assert(parse_percent("10 %").error() == ParseErr::bad_format);
    assert(parse_percent("abc%").error() == ParseErr::bad_format);
    assert(parse_percent("101%").error() == ParseErr::out_of_range);

    return 0;
}