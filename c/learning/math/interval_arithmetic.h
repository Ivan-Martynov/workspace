#ifndef _INTERVAL_ARITHMETIC_H_
#define _INTERVAL_ARITHMETIC_H_

#include <stdbool.h>

struct interval
{
    double lower_bound;
    double upper_bound;
};

struct interval make_interval(const double, const double);
struct interval make_interval_center_width(const double, const double);
struct interval make_interval_center_percentage(const double, const double);

double interval_lower_bound(const struct interval* const);
double interval_upper_bound(const struct interval* const);

double interval_width(const struct interval* const);
double interval_center(const struct interval* const);
double interval_percent(const struct interval* const);

bool is_interval_negative(const struct interval* const);
bool is_interval_positive(const struct interval* const);
bool is_interval_non_negative(const struct interval* const);
bool does_interval_span_zero(const struct interval* const);

struct interval sum_intervals(
    const struct interval* const, const struct interval* const);
struct interval sub_intervals(
    const struct interval* const, const struct interval* const);
struct interval mul_intervals(
    const struct interval* const, const struct interval* const);
struct interval div_intervals(
    const struct interval* const, const struct interval* const);

#endif //_INTERVAL_ARITHMETIC_H_
