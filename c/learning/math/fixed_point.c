#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "functions.h"

double g_x;

bool are_close_enough(const double lhs, const double rhs, const double tol)
{
    return fabs(lhs - rhs) < tol;
}

double fixed_point(
    const double func(const double), double guess, const double tol)
{
    while (true)
    {
        const double next = func(guess);
        if (are_close_enough(next, guess, tol))
        {
            return next;
        }
        guess = next;
    }
}

double golden_ratio_func(const double x)
{
    return 1.0 + 1.0 / x;

}

void golden_ratio()
{
    const double tol = 0.0001;

    const double result = fixed_point(golden_ratio_func, 1.0, tol);

    printf("Golden ratio approximation = %f\n", result);
}

double sqrt_helper(const double y)
{
    return average(y, g_x / y);
}

void square_root(const double x)
{
    g_x = x;

    const double tol = 0.0001;

    const double result = fixed_point(sqrt_helper, 1.0, tol);

    printf("Square root approximation = %f\n", result);
}

double powered_x_approx(const double x)
{
    return log(1000)/log(x);
}

int main()
{
    golden_ratio();

    square_root(3);

    printf("Cosine approximation = %f\n", fixed_point(cos, 1.0, 0.0001));

    printf("x^x = 1000 solution approximation = %f\n",
        fixed_point(powered_x_approx, 2.0, 0.0001));

    return 0;
}