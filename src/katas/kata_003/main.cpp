#include <iostream>
#include <ranges>
#include <vector>

/*
C++20 Ranges Library: https://en.cppreference.com/w/cpp/ranges

Key Views:
- views::filter: https://en.cppreference.com/w/cpp/ranges/filter_view
- views::transform: https://en.cppreference.com/w/cpp/ranges/transform_view
- views::take: https://en.cppreference.com/w/cpp/ranges/take_view
- views::common: https://en.cppreference.com/w/cpp/ranges/common_view

Motivation: Declarative, lazy data pipelines with no explicit loops.
*/
std::vector<int> input{-3, -2, -1, 0, 1, 2, 3, 4, 5};

Build a pipeline that:

Filters positive numbers (> 0)
Squares each remaining value
Takes the first 3 results
Materialize the result into std::vector<int> output.

Rules:
Use <ranges> views only (views::filter, views::transform, views::take)
No for, while, or algorithms like std::copy_if
No lambdas capturing external state
*/

std::vector<int> process_vector(const std::vector<int> &input)
{
    namespace views = std::ranges::views;

    auto pipeline = input | views::filter([](int n)
                                          { return n > 0; }) |
                    views::transform([](int n)
                                     { return n * n; }) |
                    views::take(3) | views::common;

    std::vector<int> output(std::ranges::begin(pipeline), std::ranges::end(pipeline));
    return output;
}

int main()
{
    std::vector<int> input{-3, -2, -1, 0, 1, 2, 3, 4, 5};
    const auto output = process_vector(input);

    for (int n : output)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";

    return 0;
}