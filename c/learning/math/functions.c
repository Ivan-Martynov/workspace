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

double identity(const double x)
{
    return x;
}

double square(const double x)
{
    return x * x;
}
