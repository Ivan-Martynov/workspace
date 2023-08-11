#include <stdio.h>
#include "fixed_point.h"
#include "functions.h"
#include "rat_num.h"

static double g_x;
static double g_dx;
static double (*g_f)(const double);

double newton_transform(
    const double f(const double), const double x, const double dx)
{
    return x - f(x) / deriv(f, x, dx);
}

static double newton_transform_helper(const double y)
{
    return newton_transform(g_f, y, g_dx);
}

double newton_method(
    const double f(const double), const double guess, const double dx)
{
    const double tol = 0.001;
    g_f = f;
    g_dx = dx;

    const double result = fixed_point(newton_transform_helper, guess, tol);

    return result;
}

static double sqrt_helper(const double y)
{
    return square(y) - g_x;
}

static void square_root(const double x)
{
    g_x = x;

    const double result = newton_method(sqrt_helper, 1.0, 0.01);

    printf("Square root approximation of %f ~= %f\n", x, result);
    printf("The square of %f = %f\n", result, square(result));
}

void test_rational()
{
    struct rat_num num_1 = make_rat_num(3, 4);
    struct rat_num num_2 = make_rat_num(9, -15);
    printf("Rational number 1 = ");
    print_rat_num(&num_1);
    printf("\n");
    printf("Rational number 2 = ");
    print_rat_num(&num_2);
    printf("\n");

    struct rat_num sum_1 = sum_two_rat_num(&num_1, &num_2);
    struct rat_num diff_1 = sub_two_rat_num(&num_1, &num_2);
    struct rat_num mul_1 = mul_two_rat_num(&num_1, &num_2);
    struct rat_num div_1 = div_two_rat_num(&num_1, &num_2);

    printf("Sum, difference, product and quotient = ");
    print_rat_num(&sum_1);
    printf(", ");
    print_rat_num(&diff_1);
    printf(", ");
    print_rat_num(&mul_1);
    printf(", ");
    print_rat_num(&div_1);
    printf("\n");
}

int main()
{
    square_root(3.89);

#if 0
    printf("%f\n", repeated(square, 2, 5));
    printf("%f\n", repeated(inc, 12, 5));
    printf("%f\n", repeated(inc, 1, 5));
    printf("%f\n", repeated(inc, 0, 5));
    printf("%f\n", smooth(square, 5, 0.01));
    printf("%f\n", smooth_n_fold(square, 2, 5, 0.01));
    printf("%f\n", smooth_n_fold(square, 3, 5, 0.01));
    printf("%f\n", smooth_n_fold(square, 4, 5, 0.01));
    printf("%f\n", smooth_n_fold(square, 5, 5, 0.01));
    printf("Root %f\n", general_root(2, 4));
    printf("Root %f\n", general_root(2, 258));
    //printf("%f\n", smooth_n_fold(square, 2, 5, 0.01));
    //printf("%f\n", smooth_n_fold(square, 3, 5, 0.01));
    //printf("%f\n", smooth_n_fold(square, 4, 5, 0.01));
    //printf("%f\n", smooth_n_fold(square, 5, 5, 0.01));
#endif

    test_rational();

    return 0;
}