#include <iostream>
#include "Rational.h"

void test_rat() {
    constexpr Rational<int> r1 {1, 8}, r2 {1, 2};
    Rational<int> res1 = r1 * r2;
    //res1 = 1;
    auto res2 {r1 * 2};

    // empty template: using default type (most likely int)
    constexpr Rational<> res3 {2 * r2};

    //std::swap(res1, res2);

    //Rational<std::string> str_rational {};

    std::cout << res1 << '\n';
    std::cout << res2 << '\n';
    std::cout << res3 << '\n';
}

int main() {

    test_rat();

    return 0;
}

