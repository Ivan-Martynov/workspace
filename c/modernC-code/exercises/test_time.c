#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct stats stats;

struct stats
{
    double moment[4];
};

static double timespec_diff(
    const struct timespec* const first, const struct timespec* const second)
{
    return (first->tv_sec > second->tv_sec)
               ? -timespec_diff(second, first)
               : (second->tv_sec - first->tv_sec)
                     + (second->tv_nsec - first->tv_nsec) * 1E-9;
}

static inline double stats_samples(const stats* const c)
{
    return c->moment[0];
}

static inline double stats_mean(const stats* const c)
{
    return c->moment[1];
}

static inline double stats_var(const stats* const c)
{
    return c->moment[2] / stats_samples(c);
}

static inline double stats_var_unbiased(const stats* const c)
{
    return c->moment[2] / (stats_samples(c) - 1);
}

static inline double stats_sdev(const stats* const c)
{
    return sqrt(stats_var(c));
}

static inline double stats_rsdev(const stats* const c)
{
    return sqrt(stats_var(c) / stats_mean(c));
}

static inline double stats_skew(const stats* const c)
{
    const double var = stats_var(c);
    return (c->moment[3] / pow(var, 1.5)) / stats_samples(c);
}

static inline double stats_sdev_unbiased(const stats* const c)
{
    return sqrt(stats_var_unbiased(c));
}

static inline double stats_rsdev_unbiased(const stats* const c)
{
    return stats_rsdev(c) * (1 + 1 / (4 * stats_samples(c)));
}

static inline void stats_collect(stats* c, double val, unsigned moments)
{
    const double n = stats_samples(c);
    const double n0 = n - 1;
    const double n1 = n + 1;
    const double delta0 = 1;
    const double delta = val - stats_mean(c);
    const double delta1 = delta / n1;
    const double delta2 = delta1 * delta1 * n;

    switch (moments)
    {
        default:
            c->moment[3] += (delta2 * n0 - 3 * c->moment[2]) * delta1;
        case 2:
            c->moment[2] += delta2;
        case 1:
            c->moment[1] += delta1;
        case 0:
            c->moment[0] += delta0;
    }
}

static inline void stats_collect0(stats* c, double val)
{
    stats_collect(c, val, 0);
}

static inline void stats_collect1(stats* c, double val)
{
    stats_collect(c, val, 1);
}

static inline void stats_collect2(stats* c, double val)
{
    stats_collect(c, val, 2);
}

static inline void stats_collect3(stats* c, double val)
{
    stats_collect(c, val, 3);
}

volatile uint64_t s;

static void measure_time(const uint64_t iterations)
{
    struct timespec t_dummy;
    stats s_dummy[4] = {0};

    enum
    {
        probes = 10,
        loops = 7
    };

    const uint64_t upper = iterations * iterations;
    //const uint64_t iterations = (2 << 24) - 1;

    stats statistic[loops] = {0};

    for (unsigned probe = 0; probe < probes; ++probe)
    {
        uint64_t accu0 = 0;
        uint64_t accu1 = 0;
        struct timespec t[loops + 1] = {0};

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
        for (uint64_t i = 0; i < upper; ++i)
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
        // A function call can usually not be optimized out, but an inline
        // function can.
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

        printf("s, %zu iterations, accu0 %zx, accu1 %zx\n", s, accu0, accu1);

        for (size_t moment = 1; moment < 4; ++moment)
        {
            const double mean = stats_mean(&s_dummy[moment]);
            const double sdev = stats_rsdev_unbiased(&s_dummy[moment]);
            const double skew = stats_skew(&s_dummy[moment]);
            printf(
                "%zu moments: average value of nsec\t%5.4g\t± %g%% (%g skew)\n",
                moment, mean, 100.0 * sdev, skew);
        }

        for (unsigned i = 0; i < loops; ++i)
        {
            const double diff = timespec_diff(&t[i], &t[i + 1]);
            stats_collect2(&statistic[i], diff);
        }
    }

    for (unsigned i = 0; i < loops; ++i)
    {
        const double mean = stats_mean(&statistic[i]);
        const double rsdev = stats_rsdev_unbiased(&statistic[i]);
        printf("loop %u: E(t) (sec):\t%5.2e ± %4.02f%%,\tloop body %5.2e\n", i,
            mean, 100.0 * rsdev, mean / iterations);
    }
}

void test_time()
{
    time_t result = time(NULL);

    if (result != (time_t)(-1))
    {
        printf("Seconds: %jd\n", (intmax_t)result);
    }
}

int main(const int argc, char* argv[static argc])
{
    if (argc < 2)
    {
        test_time();
    }
    else
    {
        const uint64_t iterations = strtoull(argv[1], 0, 0);
        measure_time(iterations);
    }

    return EXIT_SUCCESS;
}
