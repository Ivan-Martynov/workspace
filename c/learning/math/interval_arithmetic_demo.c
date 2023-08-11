#include <stdio.h>
#include "interval_arithmetic.h"

void debug_interval(const struct interval* const src_ptr)
{
    printf("(%f, %f), width = %f, center = %f, percent = %.2f\n",
        interval_lower_bound(src_ptr), interval_upper_bound(src_ptr),
        interval_width(src_ptr), interval_center(src_ptr),
        interval_percent(src_ptr));
}

struct interval par_ohm_ver1(
    const struct interval* const lhs, const struct interval* const rhs)
{
    const struct interval prod_temp = mul_intervals(lhs, rhs);
    const struct interval sum_temp = sum_intervals(lhs, rhs);

    return div_intervals(&prod_temp, &sum_temp);
}

struct interval par_ohm_ver2(
    const struct interval* const lhs, const struct interval* const rhs)
{
    const struct interval one = make_interval(1.0, 1.0);

    struct interval div_temp1 = div_intervals(&one, lhs);
    struct interval div_temp2 = div_intervals(&one, rhs);

    struct interval sum_temp = sum_intervals(&div_temp1, &div_temp2);

    return div_intervals(&one, &sum_temp);
}

int main()
{
    const struct interval intervals[] = {make_interval_center_percentage(6.8, 10),
        make_interval_center_percentage(4.7, 5), make_interval(-1.1, 2.3),
        make_interval_center_width(2.0, 3.0)};

    for (size_t i = 0; i < sizeof(intervals) / sizeof(*intervals); ++i)
    {
        printf("Interval %zu:\n", i);
        debug_interval(&intervals[i]);
    }

    printf("Test 1:\n");
    const struct interval test1 = div_intervals(&intervals[0], &intervals[3]);
    debug_interval(&test1);

    printf("Par 1:\n");
    const struct interval par1 = par_ohm_ver1(&intervals[0], &intervals[1]);
    debug_interval(&par1);

    printf("Par 2:\n");
    const struct interval par2 = par_ohm_ver2(&intervals[0], &intervals[1]);
    debug_interval(&par2);

    printf("Test 2:\n");
    const struct interval test2 = div_intervals(&intervals[0], &intervals[0]);
    debug_interval(&test2);

    printf("Test 3:\n");
    const struct interval test3 = div_intervals(&intervals[0], &intervals[1]);
    debug_interval(&test3);

    return 0;
}
