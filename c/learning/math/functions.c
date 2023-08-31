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
    else if (n & 1)
    {
        return x * power_to_n(x, n - 1);
    }
    else
    {
        return square(power_to_n(x, n >> 1));
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

static size_t gcd_lcm_helper(
    int a, int b, size_t target_function(const int, const int))
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

    return (a > b) ? target_function(b, a) : target_function(a, b);
}

static size_t gcd_impl(const int a, const int b)
{
    const int r = a % b;
    return r ? gcd_impl(b, r) : (size_t)b;
}

size_t gcd(int a, int b)
{
    return gcd_lcm_helper(a, b, gcd_impl);
}

static size_t lcm_impl(const int a, const int b)
{
    return a * (gcd_impl(a, b) / b);
}

size_t lcm(int a, int b)
{
    return gcd_lcm_helper(a, b, lcm_impl);
}

static size_t gcd_lcm_array_helper(
    const size_t n, int arr[static n], size_t target_function(int, int))
{
    if (!n || !arr)
    {
        return 0;
    }

    int result = (arr[0] < 0) ? -arr[0] : arr[0];
    for (size_t i = 1; i < n; ++i)
    {
        result = target_function(result, arr[i]);
    }
    return result;
}

size_t gcd_array(const size_t n, int arr[static n])
{
    return gcd_lcm_array_helper(n, arr, gcd);
}

size_t lcm_array(const size_t n, int arr[static n])
{
    return gcd_lcm_array_helper(n, arr, lcm);
}

double repeated(double f(const double), size_t n, double x)
{
    for (; n > 0; --n)
    {
        x = f(x);
    }

    return x;
}

double smooth(double f(const double), const double x, const double dx)
{
    return (f(x - dx) + f(x) + f(x + dx)) / 3.0;
}

double smooth_n_fold(double f(const double), const size_t n,
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

double deriv(double f(const double), const double x, const double dx)
{
    return (f(x + dx) - f(x)) / dx;
}
