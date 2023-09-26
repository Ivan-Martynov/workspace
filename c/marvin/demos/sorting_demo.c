#define _POSIX_C_SOURCE 199309L // For clock_gettime and friends.

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

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

static void sorting_empty(
    void sorting_function(void* const, size_t, const size_t,
        int (*)(const void* const, const void* const)),
    int compare_function(const void* const, const void* const))
{
    sorting_function(mrvn_null_ptr, 0, 0, compare_function);
    assert(mrvn_is_array_sorted(mrvn_null_ptr, 0, 0, compare_function));
}

static void sorting_one_element(void sorting_function(void* const, size_t,
    const size_t, int (*)(const void* const, const void* const)),
    int compare_function(const void* const, const void* const))
{
    double test_array[] = {DBL_MIN};
    const size_t block_size = sizeof(test_array[0]);
    const size_t array_size = sizeof(test_array) / block_size;

    sorting_function(test_array, array_size, block_size, compare_function);

    assert(mrvn_is_array_sorted(
        test_array, array_size, block_size, compare_function));
}

static double test_sorting_algorithm(const size_t run_count,
    const size_t array_size,
    void sorting_function(void* const, size_t, const size_t,
        int (*)(const void* const, const void* const)),
    int compare_function(const void* const, const void* const),
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

        const double d = mrvn_current_timespec_milliseconds();

        fill_size_t_array_random(array_size, test_array);

        sorting_function(
            test_array, array_size, block_size, compare_function);

        if (!mrvn_is_array_sorted(
                test_array, array_size, block_size, compare_function))
        {
            printf("Sort: array not sorted.\n");
        }

        mrvn_free_set_null(&test_array);

        average_time += mrvn_current_timespec_milliseconds() - d;
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
    double t1 = test_sorting_algorithm(
        run_count, sizes[0], mrvn_merge_sort, mrvn_compare_size_t, "merge");

    for (size_t i = 1; i < sizes_count; ++i)
    {
        const double n2 = sizes[i] * log(sizes[i]);
        const double t2 = test_sorting_algorithm(
            run_count, sizes[i], mrvn_merge_sort, mrvn_compare_size_t, "merge");

        printf("Test %zu: %g; %g\n", i, n2 / n1, t2 / t1);

        n1 = n2;
        t1 = t2;
    }

    sorting_empty(mrvn_merge_sort, mrvn_compare_double);
    sorting_one_element(mrvn_merge_sort, mrvn_compare_double);
}

static void test_bubble_sort(const size_t run_count, const size_t array_size)
{
    test_sorting_algorithm(
        run_count, array_size, mrvn_bubble_sort, mrvn_compare_size_t, "bubble");
    sorting_empty(mrvn_merge_sort, mrvn_compare_double);
    sorting_one_element(mrvn_merge_sort, mrvn_compare_double);
}

int main(void)
{
    test_bubble_sort(1, 100);
    test_merge_sort();

    return EXIT_SUCCESS;
}
