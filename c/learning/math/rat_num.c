#include <stdio.h>
#include "rat_num.h"
#include "functions.h"

struct rat_num make_rat_num(int n, int d)
{
    if (d < 0)
    {
        n = -n;
        d = -d;
    }

    const int g = gcd(n, d);

    return (struct rat_num) {.numerator = n / g, .denominator = d / g};
}

int get_numerator(const struct rat_num* const num_ptr)
{
    return num_ptr->numerator;
}

int get_denominator(const struct rat_num* const num_ptr)
{
    return num_ptr->denominator;
}

int print_rat_num(const struct rat_num* const num_ptr)
{
    return printf("%d/%d", get_numerator(num_ptr), get_denominator(num_ptr));
}

struct rat_num sum_two_rat_num(
    const struct rat_num* const lhs_ptr, const struct rat_num* const rhs_ptr)
{
    const double n1 = get_numerator(lhs_ptr);
    const double d1 = get_denominator(lhs_ptr);

    const double n2 = get_numerator(rhs_ptr);
    const double d2 = get_denominator(rhs_ptr);

    return make_rat_num(n1 * d2 + n2 * d1, d1 * d2);
}

struct rat_num sub_two_rat_num(
    const struct rat_num* const lhs_ptr, const struct rat_num* const rhs_ptr)
{
    const double n1 = get_numerator(lhs_ptr);
    const double d1 = get_denominator(lhs_ptr);

    const double n2 = get_numerator(rhs_ptr);
    const double d2 = get_denominator(rhs_ptr);

    return make_rat_num(n1 * d2 - n2 * d1, d1 * d2);
}

struct rat_num mul_two_rat_num(
    const struct rat_num* const lhs_ptr, const struct rat_num* const rhs_ptr)
{
    return make_rat_num(get_numerator(lhs_ptr) * get_numerator(rhs_ptr),
        get_denominator(lhs_ptr) * get_denominator(rhs_ptr));
}

struct rat_num div_two_rat_num(
    const struct rat_num* const lhs_ptr, const struct rat_num* const rhs_ptr)
{
    return make_rat_num(get_numerator(lhs_ptr) * get_denominator(rhs_ptr),
        get_denominator(lhs_ptr) * get_numerator(rhs_ptr));
}