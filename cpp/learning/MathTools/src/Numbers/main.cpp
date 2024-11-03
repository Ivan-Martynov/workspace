#include "Numbers/Rational.h"
#include "Numbers/RationalTemplated.h"
#include "Numbers/Constants.h"

#include <iostream>
#include <type_traits>

// Example of using a custom literal to produce a Rational instance using only
// unsigned long long int value, representing a non-negative numerator
// (denominator is one in this case).
static constexpr auto operator""_rat(unsigned long long int value)
{
    return Marvin::Rational {static_cast<Marvin::Rational::value_type>(value)};
}

static void test_rational()
{
    static_assert(
        Marvin::Constants::one_half <= Marvin::Constants::three_quarters,
        "1/2 should be less than 3/4");
    static_assert(Marvin::Rational {0, -3} == Marvin::Rational {0, 1});
    static_assert(Marvin::Rational {-2, -3} == Marvin::Rational {2, 3});
    static_assert(Marvin::Rational {2, -3} == Marvin::Rational {-2, 3});

    auto number {
        Marvin::Constants::three_quarters + Marvin::Constants::one_half};
    std::cout << number << "\n";

    number += 3_rat /= Marvin::Rational {5, 6} *= Marvin::Rational {2, 3};
    auto rat2 = Marvin::Rational {-number.numerator(), number.denominator()};
    rat2.invert();

    std::cout << rat2 << " => " << static_cast<double>(rat2) << "\n";

    // Won't probably compile - possible dangling reference.
    const int& ref {Marvin::Rational {17, 24}.numerator()};
    std::cout << ref << "\n";

    std::cout << Marvin::Rational {7, 12}.numerator() << "\n";
    std::cout << static_cast<float>(Marvin::Rational {2, 0}) << "\n";
    std::cout << static_cast<double>(Marvin::Rational {-3, 0}) << "\n";
    std::cout << static_cast<long double>(Marvin::Rational {0, 0}) << "\n";
}

int main()
{
    test_rational();

    return 0;
}