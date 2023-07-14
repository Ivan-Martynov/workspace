#include <stdio.h>

/**
 * @brief Calculate the exponential of a given number with given power.
 * 
 * @param value Base value.
 * @param power Power number.
 * 
 * @return double Exponential.
 * 
 * @version 0.1
 * 
 * @author Ivan Martynov
 * 
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 */
double exponentiation_simple_i(const double value, size_t power)
{
    double result = 1.0;

    for (; power > 0; --power)
    {
        result *= value;
    }

    return result;
}

double square(const double value)
{
    return value * value;
}

/**
 * @brief Calculate the exponential of a given number with given power using
 * recursive process.
 *
 * @remark Algorithm squares the result of the next iteration with power twice
 * smaller if the power is even. If the power is odd, then it multiplies the
 * value by the result of the next iteration.
 *
 * @param value Base value.
 * @param power Power number.
 *
 * @return double Exponential.
 
 * @version 0.1
 *
 * @author Ivan Martynov
 *
 * @date 2023-07-14
 *
 * @copyright Copyright (c) 2023
 */
double exponentiation_fast_recursive_i(const double value, const size_t power)
{
    if (power == 0)
    {
        return 1;
    }
    else if (power & 1)
    {
        return value * (exponentiation_fast_recursive_i(value, power - 1));
    }
    else
    {
        return square(exponentiation_fast_recursive_i(value, power >> 1));
    }
}

/**
 * @brief Calculate the exponential of a given number with given power using
 * iterative process.
 *
 * @remark During the iteration process the algorithm squares the value if the
 * power is even. If the power is odd, multiplies the result by the current
 * value. For this reason, both arguments are not constant as they are used
 * during the computation.
 *
 * @param value Base value.
 * @param power Power number.
 *
 * @return double Exponential.
 
 * @version 0.1
 *
 * @author Ivan Martynov
 *
 * @date 2023-07-14
 *
 * @copyright Copyright (c) 2023
 */
double exponentiation_fast_iterative_i(double value, size_t power)
{
    double result = 1.0;

    for (; power > 0; )
    {
        if (power & 1)
        {
            result *= value;
            --power;
        }
        else
        {
            value *= value;
            power >>= 1;
        }
    }

    return result;
}

void test_exponentiation_functions()
{
    const double values[] = {0.0, 1.0, 2.0, 5.412, -8.3, -3.0};
    const size_t powers[] = {0, 5, 2, 1, 3, 8, 6};

    const size_t value_count = sizeof(values) / sizeof(values[0]);
    const size_t power_count = sizeof(powers) / sizeof(powers[0]);

    printf("Testing factorial functions\n");
    for (size_t i = 0; i < value_count; ++i)
    {
        const double v = values[i];
        for (size_t j = 0; j < power_count; ++j)
        {
            const size_t p = powers[j];

            printf("\nexponentiation_simple_i(%f, %zu)         = %f\n", v, p,
                exponentiation_simple_i(v, p));
            printf("exponentiation_fast_recursive_i(%f, %zu) = %f\n", v, p,
                exponentiation_fast_recursive_i(v, p));
            printf("exponentiation_fast_iterative_i(%f, %zu) = %f\n", v, p,
                exponentiation_fast_iterative_i(v, p));
        }
    }
}

int main()
{
    test_exponentiation_functions();

    return 0;
}