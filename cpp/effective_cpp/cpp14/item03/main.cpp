/*
 * Makefile may use -std=c++14 as an option
 */

#include <iostream>
#include <vector>

// return size of an array: using reference to the array and no need to have
// parameter's name
template <class T, size_t N>
constexpr size_t array_size(T(&)[N]) noexcept { return N; }

template <class C>
void print_array(const C& c) {
    for (const auto& x: c)
        std::cout << x << ' ';
    std::cout << '\n';
}

template <class C, class I>
decltype(auto) auth_access(C&& c, I i) {
    return std::forward<C>(c)[i];
}

void test_array() {
    std::vector<int> v {2, 3, 8, -2, 5, 9};
    auth_access(v, 3) = 4;
    print_array(v);
}

int main() {

    test_array();

    return 0;
}

