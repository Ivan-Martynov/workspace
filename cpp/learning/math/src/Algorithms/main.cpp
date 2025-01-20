#include "Algorithms/Numerical.h"
#include "Algorithms/Sorting.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

namespace
{

auto measure_function_time(auto&& f, auto&&... pars)
{
    const auto start {std::chrono::high_resolution_clock::now()};
    std::forward<decltype(f)>(f)(std::forward<decltype(pars)>(pars)...);
    return std::chrono::high_resolution_clock::now() - start;
}

auto measure_function_time(int n, auto&& f, auto&&... pars)
{
    std::cout << "Running for " << n << " times\n";
    std::chrono::duration duration {
        std::chrono::high_resolution_clock::now()
        - std::chrono::high_resolution_clock::now()};
    for (int i {1}; i < n; ++i) {
        const auto start {std::chrono::high_resolution_clock::now()};
        std::forward<decltype(f)>(f)(std::forward<decltype(pars)>(pars)...);
        duration += std::chrono::high_resolution_clock::now() - start;
    }
    return duration;
}

#if 0
// Measuring time calling a function multiple times.
auto time_func = [](size_t n, auto&& f, auto&&... pars) {
    // using namespace std::chrono;
    std::chrono::duration duration {};
    // auto t = high_resolution_clock::now();
    //  call function n times
    for (size_t i {0}; i < n; ++i) {
        //const auto start {std::chrono::high_resolution_clock::now()};
        //std::forward<decltype(f)>(f)(std::forward<decltype(pars)>(pars)...);
        //duration += std::chrono::high_resolution_clock::now() - start;
        duration += time_func(f, pars);
    }
    return duration;
    // return high_resolution_clock::now() - t;
};
#endif

// void print_array()
template <typename T>
void print_array(const T& array)
{
    for (auto x : array) {
        std::cout << x << ", ";
    }

    std::cout << "\n";
}

[[maybe_unused]]
void test_factorial()
{
    std::cout << Marvin::factorial(5) << "\n";
    std::cout << Marvin::factorial_template<4>() << "\n";
    std::cout << Marvin::factorial_recursive(3) << "\n";
}

[[maybe_unused]]
void test_gcd()
{
    constexpr auto d {Marvin::gcd(24, 32)};
    std::cout << d << "\n";
}

void test_sorting(Marvin::container auto&& array)
{
    //print_array(array);
    // Marvin::insert_sort(array.begin(), array.end());
    // Marvin::selection_sort(array.begin(), array.end());
    //Marvin::bubble_sort(array.begin(), array.end());
    //Marvin::merge_sort(array.begin(), array.end());
    Marvin::merge_sort(array);
    //print_array(array);
    if (!Marvin::is_sorted(array.begin(), array.end())) {
        std::cout << "Array not sorted!\n";
    }
    //const auto t {time_func(Marvin::bubble_sort, array.begin(), array.end())};
    //std::cout << "Duration = " << t << "\n";
}

void test_sorting()
{
    test_sorting(std::vector {5, 2, 4, 6, 1, 3});
    test_sorting(std::vector {5, 2, 4, 7, 1, 3, 2, 6});
    test_sorting(std::vector {8, 3});
    test_sorting(std::vector {17});
    test_sorting(std::vector<int> {});
}

} // namespace

int main()
{
    //test_gcd();
    //test_factorial();
    //test_sorting();

    //const auto t {measure_function_time(test_gcd)};
    constexpr int n {1 << 16};
    const auto t {measure_function_time(n, []() { test_sorting(); })};
    std::cout << "Duration = " << t << ": " << (t.count() * 1E-9) << "s.\n";

    return 0;
}
