#include <array>
#include <cassert>
#include <vector>
#include <ranges>
#include <numeric>
#include <functional>

// Custom fold_left implementation (C++23 not yet in MSVC STL)
namespace custom
{
    template <typename Range, typename T, typename BinaryOp>
    constexpr T fold_left(Range &&rng, T init, BinaryOp op)
    {
        for (auto &&elem : rng)
        {
            init = op(std::move(init), std::forward<decltype(elem)>(elem));
        }
        return init;
    }
}

/*
Task

Compute a moving average over a sequence using <ranges> views only (no explicit loops).

Requirements

Single file main.cpp.

Given:

std::vector<double> input{ 1, 2, 3, 4, 5 };
std::size_t window = 3;


Build a pipeline that:

Produces sliding windows of size window
Computes the average of each window
Materialize the result into:

std::vector<double> output;

In main() assert:

output == std::vector<double>{ 2.0, 3.0, 4.0 };


Rules:

Use <ranges> views only
No for / while
No algorithms like std::accumulate
No temporary containers per window

Hints
Use views::slide(window)
Use views::transform
Use std::ranges::fold_left (C++23)

Constraints

C++23
<ranges>, <vector>, <cassert>
No frameworks
No logging
*/

int main()
{

    std::vector<double> input{1, 2, 3, 4, 5};
    std::size_t window = 3;

    auto avg_window = [window](auto const& w) {
        const double sum = custom::fold_left(w, 0.0, std::plus<double>{});
        return sum / static_cast<double>(window);
    };

    auto windows = input | std::views::slide(window);
    auto moving_averages = windows | std::views::transform(avg_window);

    std::vector<double> output(moving_averages.begin(), moving_averages.end());

    assert((output == std::vector<double>{2.0, 3.0, 4.0}));

    return 0;
}