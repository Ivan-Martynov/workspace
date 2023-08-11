#include <stdio.h>
#include "functions.h"

double average_safe(const double x, const double y)
{
    return x + (y - x) * 0.5;
}

double average(const double x, const double y)
{
    return (x + y) * 0.5;
}

double sum_two(const double x, const double y)
{
    return x + y;
}

double multiply_two(const double x, const double y)
{
    return x * y;
}

double inc_by_dx(const double x, const double dx)
{
    return x + dx;
}

double inc_by_two(const double x)
{
    return inc_by_dx(x, 2);
}

double inc(const double x)
{
    return inc_by_dx(x, 1);
}

double power_to_n(const double x, const size_t n)
{
    if (n == 0)
    {
        return 1;
    }
    else if (n % 2 == 0)
    {
        return square(power_to_n(x, n >> 1));
    }
    else
    {
        return x * power_to_n(x, n - 1);
    }
}

double identity(const double x)
{
    return x;
}

double square(const double x)
{
    return x * x;
}

static int gcd_impl(const int a, const int b)
{
    const int r = a % b;
    return r ? gcd_impl(b, r) : b;
}

int gcd(int a, int b)
{
    if (!a && !b)
    {
        return 0;
    }

    if (a < 0)
    {
        a = -a;
    }

    if (b < 0)
    {
        b = -b;
    }

    return (a > b) ? gcd_impl(b, a) : gcd_impl(a, b);
}

double repeated(const double f(const double), size_t n, double x)
{
    for (; n > 0; --n)
    {
        x = f(x);
    }

    return x;
}

double smooth(const double f(const double), const double x, const double dx)
{
    return (f(x - dx) + f(x) + f(x + dx)) / 3.0;
}

double smooth_n_fold(const double f(const double), const size_t n,
    const double x, const double dx)
{
    if (n == 0)
    {
        return f(x);
    }
#if 0
    else if (n == 1)
    {
        return smooth(f, x, dx);
    }
#endif
    else
    {
        return (smooth_n_fold(f, n - 1, x - dx, dx)
                   + smooth_n_fold(f, n - 1, x, dx)
                   + smooth_n_fold(f, n - 1, x + dx, dx))
               / 3.0;
    }
}

double deriv(const double f(const double), const double x, const double dx)
{
    return (f(x + dx) - f(x)) / dx;
}
