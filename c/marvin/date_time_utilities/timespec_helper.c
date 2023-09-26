#define _POSIX_C_SOURCE 199309L // For clock_gettime and friends.

#include "timespec_helper.h"

/**
 * @brief Convert timespec data into a number of type double.
 *
 * @param spec_ptr Pointer to timespec object.
 *
 * @return double Double value representing time in seconds.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
double mrvn_timespec_to_ms_double(const struct timespec* const spec_ptr)
{
    return 1000.0 * spec_ptr->tv_sec + 1E-6 * spec_ptr->tv_nsec;
}

/**
 * @brief Current time in milliseconds.
 * 
 * @return double Time in milliseconds.
 */
double mrvn_current_timespec_milliseconds(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);

    return mrvn_timespec_to_ms_double(&spec);
}

/**
 * @brief Duration in seconds between two time points represented by timespec.
 *
 * @param lhs Pointer to first object.
 * @param rhs Pointer to second object.
 *
 * @return double Time in seconds.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
double mrvn_timespec_duration_ms_double(
    const struct timespec* const lhs, const struct timespec* const rhs)
{
    return mrvn_timespec_to_ms_double(rhs) - mrvn_timespec_to_ms_double(lhs);
}
