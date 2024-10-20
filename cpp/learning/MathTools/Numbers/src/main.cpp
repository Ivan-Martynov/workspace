#include "Rational.h"

#include <iostream>

void test_rational()
{
    Marvin::Rational number {};
    std::cout << number.numerator() << "; " << number.denominator() << "\n";
}

int main()
{
    test_rational();

    return 0;
}