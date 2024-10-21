#include "Rational.h"
#include "Constants.h"

#include <iostream>

void test_rational()
{
    static_assert(
        Marvin::Constants::one_half <= Marvin::Constants::three_quarters,
        "1/2 should be less than 3/4");
    static_assert(Marvin::Rational {0, -3} == Marvin::Rational {0, 1});
    static_assert(Marvin::Rational {-2, -3} == Marvin::Rational {2, 3});
    static_assert(Marvin::Rational {2, -3} == Marvin::Rational {-2, 3});

    auto number {Marvin::Constants::three_quarters};
    number *= Marvin::Rational {5, 6} *= Marvin::Rational {2, 3};
    std::cout << number.numerator() << "/" << number.denominator() << "\n";
}

int main()
{
    test_rational();

    return 0;
}