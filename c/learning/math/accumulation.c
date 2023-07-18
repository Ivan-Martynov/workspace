#include <stdio.h>
#include <stdbool.h>

double filtered_accumulate(double start, const double end,
    const double null_value, const bool filter(const double),
    const double next_item_function(const double),
    const double term_function(const double),
    const double combine_function(const double, const double))
{
    double result = null_value;

    for (; start <= end; start = next_item_function(start))
    {
        if (filter(start))
        {
            result = combine_function(result, term_function(start));
        }
    }

    return result;
}

bool always_true(const double)
{
    return true;
}

double accumulate(double start, const double end, const double null_value,
    const double next_item_function(const double),
    const double term_function(const double),
    const double combine_function(const double, const double))
{

    return filtered_accumulate(start, end, null_value, always_true,
        next_item_function, term_function, combine_function);
}

double add_two(const double x, const double y)
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

double sum_squares(double start, const double end)
{
    return accumulate(start, end, 0.0, inc, square, add_two);
}

double factorial(const size_t n)
{
    return accumulate(1, n, 1, inc, identity, multiply_two);
}

double product_pi_term(const double n)
{
    const double next = n + 1;

    return (n * (n + 2)) / (next * next);
}

double product_pi(const size_t n)
{
    return 4 * accumulate(2, n, 1, inc_by_two, product_pi_term, multiply_two);
}

int main()
{
    double result = sum_squares(3, 4);

    printf("Test = %f\n", result);

    size_t n = 5;
    result = factorial(n);
    printf("Factorial of %zu = %f\n", n, result);

    n = 100;
    result = product_pi(n);
    printf("Pi approximation by product, %zu steps => %f\n", n, result);
    n = 10001;
    result = product_pi(n);
    printf("Pi approximation by product, %zu steps => %f\n", n, result);

    return 0;
}