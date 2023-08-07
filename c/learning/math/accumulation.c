#include <stdio.h>
#include <stdbool.h>
#include "functions.h"

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

double sum_squares(double start, const double end)
{
    return accumulate(start, end, 0.0, inc, square, sum_two);
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