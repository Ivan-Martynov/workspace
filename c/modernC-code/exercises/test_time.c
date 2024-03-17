#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct stats stats;

struct stats
{
    double moment[4];
};

struct timespec t_dummy;
stats s_dummy[4] = {0};

inline void stats_collect(stats* c, double val, unsigned moments)
{
    double n = stats_samples(c);
    double n0 = n - 1;
    double n1 = n + 1;
    double delta0 = 1;
    double delta = val - stats_mean(c);
    double delta1 = delta / n1;
    double delta2 = delta1 * delta1 * n;

    switch (moments)
    {
        case 2:
            c->moment[2] += delta2;
            break;

        case 1:
            c->moment[1] += delta1;

        case 0:
            c->moment[0] += delta0;

        default:
            c->moment[3] += (delta2 * n0 - 3 * c->moment[2]) * delta1;
            break;
    }
}

static void measure_time(void)
{
    const uint64_t iterations = (2 << 24) - 1;

    timespec_get(&t[0], TIME_UTC);
    // Volatile variable ensures that the loop is effected.
    for (volatile uint64_t i = 0; i < iterations; ++i)
    {
        // do nothing.
    }

    timespec_get(&t[1], TIME_UTC);
    // s must be volatile to ensure that the loop is effected.
    for (uint64_t i = 0; i < iterations; ++i)
    {
        s = i;
    }

    timespec_get(&t[2], TIME_UTC);
    // Opaque computation ensures that the loop is effected.
    for (uint64_t i = 0; i < iterations; ++i)
    {
        accu0 += i;
    }

    timespec_get(&t[3], TIME_UTC);
    // A function call can usually not be optimized out.
    for (uint64_t i = 0; i < iterations; ++i)
    {
        timespec_get(&t_dummy, TIME_UTC);
        accu1 += t_dummy.tv_nsec;
    }

    timespec_get(&t[4], TIME_UTC);
    // A function call can usually not be optimized out, but an inline function
    // can.
    for (uint64_t i = 0; i < iterations; ++i)
    {
        timespec_get(&t_dummy, TIME_UTC);
        stats_collect1(&s_dummy[1], t_dummy.tv_nsec);
    }
    timespec_get(&t[5], TIME_UTC);
    for (uint64_t i = 0; i < iterations; ++i)
    {
        timespec_get(&t_dummy, TIME_UTC);
        stats_collect2(&s_dummy[2], t_dummy.tv_nsec);
    }
    timespec_get(&t[6], TIME_UTC);
    for (uint64_t i = 0; i < iterations; ++i)
    {
        timespec_get(&t_dummy, TIME_UTC);
        stats_collect3(&s_dummy[2], t_dummy.tv_nsec);
    }
    timespec_get(&t[7], TIME_UTC);
}

void test_time()
{
    time_t result = time(NULL);

    if (result != (time_t)(-1))
    {
        printf("Seconds: %jd\n", (intmax_t)result);
    }
}

int main()
{
    test_time();

    return EXIT_SUCCESS;
}
