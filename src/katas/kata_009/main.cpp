
#include <array>
#include <span>
#include <cassert>

/*
Task

Normalize an array of floats in-place using std::span (no copies).

Requirements

Single file main.cpp.

Implement:

// Returns false if input is empty or max == min (cannot normalize)
bool normalize_0_1(std::span<float> xs);


Rules:

If xs.empty() → return false
Compute min and max of the span
If max == min → return false
Otherwise, transform each element in-place:

x = (x - min) / (max - min)

No allocations.

In main():

Test 1:

std::array<float, 5> a{ 10.f, 20.f, 15.f, 20.f, 10.f };


normalize_0_1(a) returns true

Assert results are exactly:

10 → 0
20 → 1
15 → 0.5

Test 2: all-equal input returns false:

std::array<float, 3> b{ 2.f, 2.f, 2.f };

Test 3: empty span returns false:

std::span<float> empty{};

Use assert only (no logging).

Constraints

C++23
Use std::span
No frameworks
No extra helpers beyond standard headers

*/

bool normalize_0_1(std::span<float> xs)
{
    // Returns false if input is empty or max == min
    if (xs.empty())
        return false;

    float min = xs[0];
    float max = xs[0];

    // Find min and max    
    for (float x : xs)
    {
        if (x < min)
            min = x;
        if (x > max)
            max = x;
    }

    // Check if max equals min
    if (max == min)
        return false;

    float range = max - min;

    // Normalize in-place
    for (float& x : xs)
    {
        x = (x - min) / range;
    }

    return true;
}


int main()
{
    // Test 1
    {
        std::array<float, 5> arr{ 10.f, 20.f, 15.f, 20.f, 10.f };
        bool result = normalize_0_1(arr);
        assert(result == true);
        assert(arr[0] == 0.f);
        assert(arr[1] == 1.f);
        assert(arr[2] == 0.5f);
        assert(arr[3] == 1.f);
        assert(arr[4] == 0.f);
    }

    // Test 2: all-equal input returns false
    {
        std::array<float, 3> arr2{ 2.f, 2.f, 2.f };
        bool result = normalize_0_1(arr2);
        assert(result == false);
    }

    // Test 3: empty span returns false
    {
        std::span<float> empty{};
        bool result = normalize_0_1(empty);
        assert(result == false);
    }

    return 0;
}