#include "comparison_helper.h"

#include <stddef.h>

/**
 * @brief Compare two size_t values, using void pointers.
 *
 * @param first_ptr Pointer to first value.
 * @param second_ptr Pointer to second value.
 *
 * @return int If first value is greater, then function returns 1. If it is
 * smaller, then -1. Otherwize zero.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
int mrvn_compare_size_t(
    const void* const first_ptr, const void* const second_ptr)
{
    const size_t a = *(const size_t* const)first_ptr;
    const size_t b = *(const size_t* const)second_ptr;

    return (a > b) ? 1 : (a < b) ? -1 : 0;
}

/**
 * @brief Compare two double values, using void pointers.
 *
 * @param first_ptr Pointer to first value.
 * @param second_ptr Pointer to second value.
 *
 * @return int If first value is greater, then function returns 1. If it is
 * smaller, then -1. Otherwize zero.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
int mrvn_compare_double(
    const void* const first_ptr, const void* const second_ptr)
{
    const double a = *(const double* const)first_ptr;
    const double b = *(const double* const)second_ptr;

    return (a > b) ? 1 : (a < b) ? -1 : 0;
}

/**
 * @brief Compare two floats using precision value @eps.
 *
 * @param lhs First value.
 * @param rhs Second value.
 * @param eps Precision.
 *
 * @return true Values are equal within the given precision.
 * @return false Values are not equal.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
bool mrvn_almost_equal_float_eps(
    const float lhs, const float rhs, const float eps)
{
    return (lhs < rhs) ? rhs - lhs < eps : lhs - rhs < eps;
}
