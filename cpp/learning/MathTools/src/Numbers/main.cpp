#include "Numbers/Rational.h"
#include "Numbers/Complex.h"
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

static auto operator""_complex(unsigned long long int value)
{
    return Marvin::Complex {static_cast<Marvin::Complex::value_type>(value)};
}

static void test_rational()
{
    static_assert(
        Marvin::Constants::one_half <= Marvin::Constants::three_quarters,
        "1/2 should be less than 3/4");
    static_assert(Marvin::Rational {0, -3} == Marvin::Rational {0, 1});
    static_assert(Marvin::Rational {-2, -3} == Marvin::Rational {2, 3});
    static_assert(Marvin::Rational {2, -3} == Marvin::Rational {-2, 3});

    auto&& number {
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

static Marvin::Complex test_function_return()
{
    Marvin::Complex complex {27.379, -529.2038};

    return complex;
}

static void test_complex()
{
    Marvin::Complex complex_01 {2, 8.2f};

    std::cout << "Complex: " << complex_01.real() << "; " << complex_01.imag()
              << "\n";
    Marvin::ComplexTemplated<int> complex_02 {-4, 7};
    Marvin::ComplexTemplated<float> complex_03 {
        static_cast<float>(complex_02.real()),
        static_cast<float>(complex_02.imag())};

    Marvin::Complex complex_04 {Marvin::Complex {-3.8, 12.5}};
    Marvin::Complex complex_05 {test_function_return()};
    Marvin::Complex complex_06 {37_complex};
}

int main()
{
    test_rational();
    std::cout << "\n";
    test_complex();

    return 0;
}