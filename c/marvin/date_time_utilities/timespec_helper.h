#ifndef _H_TIMESPEC_HELPER_H_
#define _H_TIMESPEC_HELPER_H_

#include <time.h>

double mrvn_timespec_to_seconds_double(const struct timespec* const spec_ptr);

double mrvn_timespec_duration_seconds_double(
    const struct timespec* const lhs,
    const struct timespec* const rhs);

#endif //_H_TIMESPEC_HELPER_H_
