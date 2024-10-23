#include "Algorithms/Numerical.h"

#include <iostream>

static void test_factorial()
{
    std::cout << Marvin::factorial(5) << "\n";
    std::cout << Marvin::factorial_template<4>() << "\n";
    std::cout << Marvin::factorial_recursive(3) << "\n";
}

static void test_gcd()
{
    constexpr auto d {Marvin::gcd(24, 32)};
    std::cout << d << "\n";
}

int main()
{
    test_gcd();
    test_factorial();

    return 0;
}
