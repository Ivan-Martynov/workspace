#ifndef _H_TIMESPEC_HELPER_H_
#define _H_TIMESPEC_HELPER_H_

#include <time.h>

/**
 * @brief Convert timespec data into a number of type double.
 */
double mrvn_timespec_to_ms_double(const struct timespec* const spec_ptr);

/**
 * @brief Current time in milliseconds.
 */
double mrvn_current_timespec_milliseconds(void);

/**
 * @brief Duration in seconds between two time points represented by timespec.
 */
double mrvn_timespec_duration_ms_double(
    const struct timespec* const lhs, const struct timespec* const rhs);

#endif //_H_TIMESPEC_HELPER_H_
