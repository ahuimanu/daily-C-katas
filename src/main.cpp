#include <cassert>
#include <cctype>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

/*
C++20 Daily Kata: std::optional as a Boundary Type

Task
Model a “data may be missing” boundary using std::optional, with no sentinels and no exceptions.

Requirements
Single file main.cpp.

Implement:
struct WxSample {
  int wind_dir_deg;   // 0..360
  int wind_kt;        // >= 0
};

std::optional<WxSample> parse_wx(std::string_view line);


Input format (strict):
"DDD/SS" where:

DDD is exactly 3 digits, 000–360
/ separator
SS is exactly 2 digits, 00–99

No whitespace anywhere.

parse_wx returns:
WxSample for valid input
std::nullopt for any invalid input

Validation:
In main() use assert to verify:

Valid:
"090/12" → {90, 12}
"360/00" → {360, 0}
"000/99" → {0, 99}

Invalid (must be nullopt):

""  (empty string)
"90/12" (dir not 3 digits)
"361/10" (dir out of range)
"090/1" (speed not 2 digits)
"090-12" (wrong separator)
"090/12 " (whitespace)
"09A/12" (non-digit)

Constraints:

C++23 only
Use std::optional
No exceptions
No sentinel values
No extra logging

*/

struct WxSample
{
    int wind_dir_deg; // 0..360
    int wind_kt;      // >= 0
};

std::optional<WxSample> parse_wx(std::string_view line);

std::optional<WxSample> parse_wx(std::string_view line)
{

    /*
    Input format (strict):
    "DDD/SS" where:

    DDD is exactly 3 digits, 000–360
    / separator
    SS is exactly 2 digits, 00–99
    */

    // Check length and separator
    if (line.size() != 6 || line[3] != '/')
        return std::nullopt;

    // Check that all other characters are digits
    for (size_t i = 0; i < line.size(); ++i)
    {
        if (i == 3)
            continue; // skip separator
        if (!std::isdigit(line[i]))
            return std::nullopt;
    }

    // For this kata, parsing must be:
    // exception-free
    // allocation-free
    // explicit    
    int wind_dir_deg = 0;
    int wind_kt = 0;

    auto [p1, ec1] = std::from_chars(line.data(), line.data() + 3, wind_dir_deg);
    auto [p2, ec2] = std::from_chars(line.data() + 4, line.data() + 6, wind_kt);
    
    if (ec1 != std::errc{} || ec2 != std::errc{})
        return std::nullopt;

    if (wind_dir_deg < 0 || wind_dir_deg > 360)
        return std::nullopt;

    if (wind_kt < 0 || wind_kt > 99)
        return std::nullopt;

    return WxSample{wind_dir_deg, wind_kt};
}

int main()
{

    std::cout << "Testing parse_wx function\n" << std::endl;

    // Valid cases
    {
        auto result = parse_wx("090/12");
        assert(result.has_value());
        assert(result->wind_dir_deg == 90);
        assert(result->wind_kt == 12);
    }
    {
        auto result = parse_wx("360/00");
        assert(result.has_value());
        assert(result->wind_dir_deg == 360);
        assert(result->wind_kt == 0);
    }
    {
        auto result = parse_wx("000/99");
        assert(result.has_value());
        assert(result->wind_dir_deg == 0);
        assert(result->wind_kt == 99);
    }

    // Invalid cases
    assert(parse_wx("") == std::nullopt);               // empty string
    assert(parse_wx("90/12") == std::nullopt);          // dir not 3 digits
    assert(parse_wx("361/10") == std::nullopt);        // dir out of range
    assert(parse_wx("090/1") == std::nullopt);          // speed not 2 digits
    assert(parse_wx("090-12") == std::nullopt);        // wrong separator
    assert(parse_wx("090/12 ") == std::nullopt);        // whitespace
    assert(parse_wx("09A/12") == std::nullopt);        // non-digit

    return 0;
}