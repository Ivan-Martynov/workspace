/*
 * Makefile may use -std=c++14 as an option
 */

#include <iostream>

// return size of an array: using reference to the array and no need to have
// parameter's name
template <class T, size_t N>
constexpr size_t array_size(T(&)[N]) noexcept { return N; }

void test_array() {
    int key_vals[] = {1, 3, 7, 9, 11, 22, 35};
    constexpr size_t n {array_size(key_vals)};
    int mapped_vals[n];
    for (size_t i {0}; i < n; ++i)
        mapped_vals[i] = key_vals[i];
    std::cout << n << " elements:\n";
    for (const auto v: mapped_vals)
        std::cout << v << ' ';
    std::cout << '\n';
}

int main() {

    test_array();

    return 0;
}

