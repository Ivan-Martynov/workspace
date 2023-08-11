#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "functions.h"

static double g_x;
static size_t g_root;

static bool are_close_enough(
    const double lhs, const double rhs, const double tol)
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

static double golden_ratio_func(const double x)
{
    return 1.0 + 1.0 / x;

}

static void golden_ratio()
{
    const double tol = 0.0001;

    const double result = fixed_point(golden_ratio_func, 1.0, tol);

    printf("Golden ratio approximation = %f\n", result);
}

static double damping(
    const double f(const double), const double x, const double alpha)
{
    return (1 - alpha) * f(x) + alpha * x;
}

static double general_root_helper(const double y)
{
    return g_x / power_to_n(y, g_root - 1);
}

static double general_root_helper2(const double y)
{
    return damping(general_root_helper, y, (g_root - 1.0) / g_root);
}

double general_root(const double x, const size_t n_th)
{
    g_x = x;
    g_root = n_th;

    const double tol = 0.0001;

    const double result = fixed_point(general_root_helper2, 1.0, tol);
}

static double sqrt_helper(const double y)
{
    return average(y, g_x / y);
}

static void square_root(const double x)
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

//int main()
//{
//    golden_ratio();
//
//    square_root(3);
//
//    printf("Cosine approximation = %f\n", fixed_point(cos, 1.0, 0.0001));
//
//    printf("x^x = 1000 solution approximation = %f\n",
//        fixed_point(powered_x_approx, 2.0, 0.0001));
//
//    return 0;
//}