#define _POSIX_C_SOURCE 199309L // For clock_gettime and friends.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

typedef struct stats stats;
struct stats
{
    double moment[5];
};

static inline double timespec_diff(
    const struct timespec* const first, const struct timespec* const second)
{
    return (second->tv_sec - first->tv_sec)
           + (second->tv_nsec - first->tv_nsec) * 1E-9;
}

static inline double timespec_diff_abs(
    const struct timespec* const first, const struct timespec* const second)
{
    return fabs(timespec_diff(first, second));
}

static inline double sqr(const double value)
{
    return value * value;
}

static inline double cube(const double value)
{
    return value * sqr(value);
}

static void moments_from_samples(
    stats* const moments_ptr, const size_t n, const double samples[static n])
{
    for (size_t m = 0; m < 4; ++m)
    {
        moments_ptr->moment[m] = 0;
    }

    for (size_t i = 0; i < n; ++i)
    {
        ++moments_ptr->moment[0];

        moments_ptr->moment[1] += samples[i];

        const double sqr_value = sqr(samples[i]);
        moments_ptr->moment[2] += sqr_value;

        moments_ptr->moment[3] += sqr_value * samples[i];

        moments_ptr->moment[4] += sqr_value * sqr_value;
    }
}

static double mean(const size_t n, const double samples[static n])
{
    assert(n > 0);

    double result = 0;

    for (size_t i = 0; i < n; ++i)
    {
        result += samples[i];
    }

    return result / n;
}

static double deviation(const size_t n, const double samples[static n])
{
    assert(n > 1);

    const double m = mean(n, samples);

    double result = 0;
    for (size_t i = 0; i < n; ++i)
    {
        result += sqr(samples[i] - m);
    }

    return result / (n - 1);
}

static double standard_deviation(const size_t n, const double samples[static n])
{
    return sqrt(deviation(n, samples));
}

static double skewness(const size_t n, const double samples[static n])
{
    assert(n > 1);

    const double m = mean(n, samples);

    double result = 0;
    const double std_dev = standard_deviation(n, samples);
    for (size_t i = 0; i < n; ++i)
    {
        result += cube(samples[i] - m);
        //result += cube((samples[i] - m) / std_dev);
    }

    const double b = result / cube(std_dev);

    return n * b / ((n - 1) * (n - 2));
}

static double kurtosis(const size_t n, const double samples[static n])
{
    assert(n > 3);

    const double m = mean(n, samples);

    double result = 0;
    //const double std_dev = standard_deviation(n, samples);
    double dev = 0;
    for (size_t i = 0; i < n; ++i)
    {
        result += sqr(sqr(samples[i] - m));
        dev += sqr(samples[i] - m);
    }
    //result /= n;
    dev *= dev;
    //const double g = result / sqr(sqr(std_dev));
    const double g = result / dev;

    // return (result / n) / ((n - 1) * sqr(sqr(std_dev)));
    return (n * (n + 1) * (n - 1) * g) / ((n - 2) * (n - 3));
    //return ((n + 1) * g + 6) * (n - 1) / ((n - 2) * (n - 3));
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

    // Running average. Using n + 1 since number of values has not been updated
    // yet.
    const double delta1 = (val - stats_mean(c)) / (n + 1);
    const double delta2 = delta1 * delta1 * n;

    switch (moments)
    {
        default:
            c->moment[3] += (delta2 * (n - 1) - 3 * c->moment[2]) * delta1;
            __attribute__ ((fallthrough));
        case 2:
            c->moment[2] += delta2;
            __attribute__ ((fallthrough));
        case 1:
            c->moment[1] += delta1;
            __attribute__ ((fallthrough));
        case 0:
            ++c->moment[0];
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

static int get_time_spec(struct timespec* timespec_ptr)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                    \
    || defined(__CYGWIN__)
    return clock_gettime(CLOCK_REALTIME, timespec_ptr);
#else
    return timespec_get(timespec_ptr, TIME_UTC);
#endif
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

        get_time_spec(&t[0]);
        // Volatile variable ensures that the loop is effected.
        for (volatile uint64_t i = 0; i < iterations; ++i)
        {
            // do nothing.
        }

        get_time_spec(&t[1]);
        // s must be volatile to ensure that the loop is effected.
        for (uint64_t i = 0; i < iterations; ++i)
        {
            s = i;
        }

        get_time_spec(&t[2]);
        // Opaque computation ensures that the loop is effected.
        for (uint64_t i = 0; i < upper; ++i)
        {
            accu0 += i;
        }

        get_time_spec(&t[3]);
        // A function call can usually not be optimized out.
        for (uint64_t i = 0; i < iterations; ++i)
        {
            get_time_spec(&t_dummy);
            accu1 += t_dummy.tv_nsec;
        }

        get_time_spec(&t[4]);
        // A function call can usually not be optimized out, but an inline
        // function can.
        for (uint64_t i = 0; i < iterations; ++i)
        {
            get_time_spec(&t_dummy);
            stats_collect1(&s_dummy[1], t_dummy.tv_nsec);
        }
        get_time_spec(&t[5]);
        for (uint64_t i = 0; i < iterations; ++i)
        {
            get_time_spec(&t_dummy);
            stats_collect2(&s_dummy[2], t_dummy.tv_nsec);
        }
        get_time_spec(&t[6]);
        for (uint64_t i = 0; i < iterations; ++i)
        {
            get_time_spec(&t_dummy);
            stats_collect3(&s_dummy[2], t_dummy.tv_nsec);
        }
        get_time_spec(&t[7]);

        printf(
            "s, %zu iterations, accu0 %zx, accu1 %zx\n", (s + 1), accu0, accu1);

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
            const double diff = timespec_diff_abs(&t[i], &t[i + 1]);
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

static void test_statistics()
{
    const double samples[] = {0.7, 1.0, 1.2, 0.6, 1.3, 0.1, 0.8, 0.3, 0.4, 0.9,
        0.5, 0.2, 0.6, 0.4, 0.4, 0.5, 0.5, 0.4, 0.6, 0.6};
    const size_t n = sizeof(samples) / sizeof(samples[0]);

    const double m = mean(n, samples);
    const double dev = deviation(n, samples);
    const double std_dev = standard_deviation(n, samples);
    const double skew = skewness(n, samples);
    const double kurt = kurtosis(n, samples);

    stats moments = {0};
    moments_from_samples(&moments, n, samples);

    for (size_t i = 0; i < 5; ++i)
    {
        printf("Moment[%zu] = %g => %g\n", i, moments.moment[i],
            moments.moment[i] / n);
    }

    printf("Mean = %g, deviation = %g, standard deviation = %g, relative "
           "standard deviation ~ %.2g%%, skewness = %g, kurtosis = %g\n",
        m, dev, std_dev, (std_dev / m) * 100.0, skew, kurt);
}

int main(const int argc, char* argv[static argc])
{
    if (argc < 2)
    {
        test_time();
        test_statistics();
    }
    else
    {
        const uint64_t iterations = strtoull(argv[1], 0, 0);
        measure_time(iterations);
    }

    return EXIT_SUCCESS;
}
