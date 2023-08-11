#include <stdio.h>
#include "functions.h"

int main()
{
    const double dx = 1E-6;
    double x = 5.0;
    printf("Derivative of square at %f = %f\n", x, deriv(square, 5, dx));

    return 0;
}
