#include <stdio.h>
#include <stdarg.h>
#include <math.h>

/**
 * @brief Helper function to compute a square (value multiplied by itself).
 * 
 * @param value Value to square.
 * 
 * @return double Squared value.
 * 
 * @version 0.1
 * 
 * @date 2024-03-01
 */
static double sqr(const double value)
{
    return value * value;
}

/**
 * @brief Calculate standard deviation using variadic number of arguments.
 * 
 * @param count Number of arguments to read.
 * @param ... Arguments.
 * 
 * @return double Standard deviation.
 * 
 * @version 0.1
 * 
 * @date 2024-03-01
 */
static double std_dev_variadic(const size_t count, ...)
{
    if (count < 1)
    {
        return 0.0;
    }

    va_list arguments_ptr;
    va_start(arguments_ptr, count);

    va_list arguments_copy_ptr;
    va_copy(arguments_copy_ptr, arguments_ptr);

    double mean = 0.0;
    for (size_t i = 0; i < count; ++i)
    {
        mean += va_arg(arguments_ptr, double);
    }
    va_end(arguments_ptr);

    mean /= count;

    double standard_deviation = 0.0;
    for (size_t i = 0; i < count; ++i)
    {
        standard_deviation += sqr(va_arg(arguments_copy_ptr, double) - mean);
    }
    va_end(arguments_copy_ptr);

    return sqrt(standard_deviation / count);
}

int main()
{
    printf("%f\n", std_dev_variadic(0));
    printf("%f\n", std_dev_variadic(2, 30.0, 40.0));
    printf("%f\n", std_dev_variadic(4, 25.0, 27.3, 26.9, 25.7));

    return 0;
}
