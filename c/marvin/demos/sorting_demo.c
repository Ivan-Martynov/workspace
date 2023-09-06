#define _POSIX_C_SOURCE 199309L // For clock_gettime and friends.

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../algorithms/sorting/sorting_algorithms.h"
#include "../numerics/comparison_utilities/comparison_helper.h"
#include "../numerics/pseudo_random_utilities/random_generator_helper.h"
#include "../type_utilities/type_constants.h"
#include "../date_time_utilities/timespec_helper.h"
#include "../memory_utilities/memory_management_helper.h"

#if 0
static void print_size_t_array(
    const size_t element_count, size_t arr[static element_count])
{
    for (size_t i = 0; i < element_count; ++i)
    {
        printf("%zu ", arr[i]);
    }
    printf("\n");
}
#endif

static void fill_size_t_array_random(size_t n, size_t arr[static n])
{
    while (n--)
    {
#if 0
        arr[n] = mrvn_simple_rand_size_t_between(20UL, 900UL);
#else
        arr[n] = rand();
#endif
    }
}

static double test_sorting_algorithm(const size_t run_count,
    const size_t array_size,
    void sorting_function(void* const, size_t, const size_t,
        int compare_function(const void* const, const void* const)),
        const char algorithm_name[static 1])
{
    assert(run_count > 0);
    assert(array_size > 0);

    mrvn_seed_simple_generator_default();

    double average_time = 0.0;

    for (size_t j = 0; j < run_count; ++j)
    {
        const size_t block_size = sizeof(size_t);

        size_t* test_array = malloc(array_size * block_size);
        assert(sizeof(test_array[0]) == block_size);

        struct timespec spec;
        clock_gettime(CLOCK_MONOTONIC, &spec);

        double d = mrvn_timespec_to_ms_double(&spec);

        fill_size_t_array_random(array_size, test_array);

        sorting_function(
            test_array, array_size, block_size, mrvn_compare_size_t);

        if (!mrvn_is_array_sorted(
                test_array, array_size, block_size, mrvn_compare_size_t))
        {
            printf("Sort: array not sorted.\n");
        }

        mrvn_free_set_null(&test_array);

        clock_gettime(CLOCK_MONOTONIC, &spec);

        const double duration = mrvn_timespec_to_ms_double(&spec) - d;

        average_time += duration;
    }

    printf("Sorting (%s) %zu elements ~ %g ms.\n", algorithm_name, array_size,
        average_time / run_count);

    return average_time / run_count;
}

static void test_merge_sort()
{
    const size_t run_count = 100;
    const size_t sizes[] = {10, 100, 1000, 10000};
    const size_t sizes_count = sizeof(sizes) / sizeof(sizes[0]);

    double n1 = sizes[0] * log(sizes[0]);
    double t1 = test_sorting_algorithm(run_count, sizes[0], mrvn_merge_sort, "merge");

    for (size_t i = 1; i < sizes_count; ++i)
    {
        const double n2 = sizes[i] * log(sizes[i]);
        const double t2 = test_sorting_algorithm(
            run_count, sizes[i], mrvn_merge_sort, "merge");

        printf("Test %zu: %g; %g\n", i, n2 / n1, t2 / t1);

        n1 = n2;
        t1 = t2;
    }
}

static double test_bubble_sort(const size_t run_count, const size_t array_size)
{
    return test_sorting_algorithm(
        run_count, array_size, mrvn_bubble_sort, "bubble");
}

int main(void)
{
    test_bubble_sort(1, 100);
    test_merge_sort();

    return EXIT_SUCCESS;
}
