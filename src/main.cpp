#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

/*
Given the input vector:
std::vector<int> input{ -3, -2, -1, 0, 1, 2, 3, 4, 5 };

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

    auto pipeline = input | views::filter([](int n){ return n > 0; }) 
                          | views::transform([](int n){ return n * n; }) 
                          | views::take(3) 
                          | views::common;

    std::vector<int> output(std::ranges::begin(pipeline), std::ranges::end(pipeline));
    return output;
}

int main()
{
    std::vector<int> input{-3, -2, -1, 0, 1, 2, 3, 4, 5};
    const auto output = process_vector(input);
    const auto validator = std::vector<int>{1, 4, 9};
    assert(output == validator && "Output does not match expected values");

    return 0;
}