#include "interval_arithmetic.h"
#include <math.h>
#include <stdio.h>

struct interval make_interval(const double lower, const double upper)
{
    return (struct interval) {.lower_bound = lower, .upper_bound = upper};
}

struct interval make_interval_center_width(
    const double center, const double width)
{
    return make_interval(center - width, center + width);
}

struct interval make_interval_center_percentage(
    const double center, const double percent)
{
    return make_interval_center_width(center, center * percent * 0.01);
}

double interval_lower_bound(const struct interval* const src_ptr)
{
    return src_ptr->lower_bound;
}

double interval_upper_bound(const struct interval* const src_ptr)
{
    return src_ptr->upper_bound;
}

double interval_width(const struct interval* const src_ptr)
{
    return (interval_upper_bound(src_ptr) - interval_lower_bound(src_ptr))
           * 0.5;
}

double interval_center(const struct interval* const src_ptr)
{
    return (interval_upper_bound(src_ptr) + interval_lower_bound(src_ptr))
           * 0.5;
}

double interval_percent(const struct interval* const src_ptr)
{
    return (100.0 * interval_width(src_ptr)) / interval_center(src_ptr);
}

bool is_interval_negative(const struct interval* const src_ptr)
{
    return (interval_lower_bound(src_ptr) < 0)
           && (interval_upper_bound(src_ptr) < 0);
}

bool is_interval_positive(const struct interval* const src_ptr)
{
    return (interval_lower_bound(src_ptr) > 0)
           && (interval_upper_bound(src_ptr) > 0);
}

bool is_interval_non_negative(const struct interval* const src_ptr)
{
    return (interval_lower_bound(src_ptr) >= 0)
           && (interval_upper_bound(src_ptr) >= 0);
}

bool does_interval_span_zero(const struct interval* const src_ptr)
{
    return (interval_lower_bound(src_ptr) <= 0)
           && (interval_upper_bound(src_ptr) >= 0);
}

struct interval sum_intervals(
    const struct interval* const lhs, const struct interval* const rhs)
{
    return make_interval(interval_lower_bound(lhs) + interval_lower_bound(rhs),
        interval_upper_bound(lhs) + interval_upper_bound(rhs));
}

struct interval sub_intervals(
    const struct interval* const lhs, const struct interval* const rhs)
{
    return make_interval(interval_lower_bound(lhs) - interval_lower_bound(rhs),
        interval_upper_bound(lhs) - interval_upper_bound(rhs));
}

static struct interval mul_intervals_non_negative_lhs(
    const struct interval* const lhs, const struct interval* const rhs)
{
    const double x_lower = interval_lower_bound(lhs);
    const double x_upper = interval_upper_bound(lhs);
    const double y_lower = interval_lower_bound(rhs);
    const double y_upper = interval_upper_bound(rhs);

    if (is_interval_non_negative(rhs))
    {
        return make_interval(x_lower * y_lower, x_upper * y_upper);
    }
    else if (is_interval_negative(rhs))
    {
        return make_interval(x_upper * y_lower, x_lower * y_upper);
    }
    else
    {
        return make_interval(x_upper * y_lower, x_upper * y_upper);
    }
}

static struct interval mul_intervals_negative_lhs(
    const struct interval* const lhs, const struct interval* const rhs)
{
    const double x_lower = interval_lower_bound(lhs);
    const double x_upper = interval_upper_bound(lhs);
    const double y_lower = interval_lower_bound(rhs);
    const double y_upper = interval_upper_bound(rhs);

    if (is_interval_non_negative(rhs))
    {
        return make_interval(x_lower * y_upper, x_upper * y_lower);
    }
    else if (is_interval_negative(rhs))
    {
        return make_interval(x_upper * y_upper, x_lower * y_lower);
    }
    else
    {
        return make_interval(x_lower * y_upper, x_lower * y_lower);
    }
}

static struct interval mul_intervals_lhs_spans_zero(
    const struct interval* const lhs, const struct interval* const rhs)
{
    const double x_lower = interval_lower_bound(lhs);
    const double x_upper = interval_upper_bound(lhs);
    const double y_lower = interval_lower_bound(rhs);
    const double y_upper = interval_upper_bound(rhs);

    if (is_interval_non_negative(rhs))
    {
        return make_interval(x_lower * y_upper, x_upper * y_upper);
    }
    else if (is_interval_negative(rhs))
    {
        return make_interval(x_upper * y_lower, x_lower * y_lower);
    }
    else
    {
        const double p1 = x_lower * y_lower;
        const double p2 = x_lower * y_upper;
        const double p3 = x_upper * y_lower;
        const double p4 = x_upper * y_upper;

        const double x = fmin(fmin(fmin(p1, p2), p3), p4);
        const double y = fmax(fmax(fmax(p1, p2), p3), p4);

        return make_interval(x, y);
    }
}

struct interval mul_intervals(
    const struct interval* const lhs, const struct interval* const rhs)
{
    if (is_interval_non_negative(lhs))
    {
        return mul_intervals_non_negative_lhs(lhs, rhs);
    }
    else if (is_interval_negative(lhs))
    {
        return mul_intervals_negative_lhs(lhs, rhs);
    }
    else
    {
        return mul_intervals_lhs_spans_zero(lhs, rhs);
    }
}

struct interval div_intervals(
    const struct interval* const lhs, const struct interval* const rhs)
{
    if (does_interval_span_zero(rhs))
    {
        fprintf(stderr, "Cannot divide by interval with zero span. Returning "
                        "(-INFINITY, INFINITY) interval\n");
        return make_interval(-INFINITY, INFINITY);
    }
    else
    {
        const struct interval temp = make_interval(
            1.0 / interval_upper_bound(rhs), 1.0 / interval_lower_bound(rhs));

        return mul_intervals(lhs, &temp);
    }
}
