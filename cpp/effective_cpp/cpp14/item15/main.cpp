/*
 * Makefile may use -std=c++14 as an option
 */

#include <iostream>
#include "Point.h"

// in C++14 we can use loops in constexpr functions
constexpr auto pow(const int base, const int exp) noexcept {
    auto res = 1;
    for (auto i = 0; i < exp; ++i)
        res *= base;
    return res;
}

template <class C>
void print_array(const C& c) {
    for (const auto& x: c)
        std::cout << x << ' ';
    std::cout << '\n';
}

void test_pow() {
    constexpr auto n1 = 3, n2 = 5;
    std::cout << n1 << " ^ " << n2 << " = " << pow(n1, n2) << '\n';
    std::cout << n2 << " ^ " << n1 << " = " << pow(n2, n1) << '\n';
}

constexpr Point mid_point(const Point& p1, const Point& p2) noexcept {
    return { (p1.x() + p2.x()) * 0.5, (p1.y() + p2.y()) * 0.5 };
}

constexpr Point reflection(const Point& p) noexcept {
    Point res;
    res.set_x(-p.x());
    res.set_y(-p.y());
    return res;
}

void test_point() {
    constexpr Point p1 {9.4, 27.7};
    constexpr Point p2 {28.8, 5.3};
    constexpr auto pm = mid_point(p1, p2);
    constexpr auto pr = reflection(pm);

    std::cout << "p1 = " << p1 << '\n';
    std::cout << "p2 = " << p2 << '\n';
    std::cout << "midpoint(p1, p2) = " << pm << '\n';

    std::cout << "reflection(pm) = " << pr << '\n';
    std::cout << "dist_from_origin(pr) = " << pr.dist_from_origin() << '\n';
}

int main() {

    //test_pow();
    test_point();

    return 0;
}

