#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../algorithms/sorting/sorting_algorithms.h"
#include "../comparison_utilities/comparison_helper.h"
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
        arr[n] = rand();
    }
}

static void test_bubble_sort(const size_t run_count, const size_t array_size)
{
    assert(run_count > 0);
    assert(array_size > 0);

    srand(time(mrvn_null_ptr));

    double average_time = 0.0;

    for (size_t j = 0; j < run_count; ++j)
    {
        size_t* test_array = malloc(array_size * sizeof(size_t));
        assert(sizeof(test_array[0]) == sizeof(size_t));

        struct timespec spec;
        clock_gettime(CLOCK_MONOTONIC, &spec);

        double d = mrvn_timespec_to_seconds_double(&spec);

        fill_size_t_array_random(array_size, test_array);

        mrvn_bubble_sort_generic(
            test_array, array_size, sizeof(size_t), mrvn_compare_size_t);

        mrvn_free_set_null((void*)&test_array);

        clock_gettime(CLOCK_MONOTONIC, &spec);

        const double duration = mrvn_timespec_to_seconds_double(&spec) - d;

        average_time += duration;
    }

    printf("Average time for bubble sorting %zu element ~ %g ms.\n", array_size,
        average_time / run_count);
}

int main(void)
{
    test_bubble_sort(1, 1E6);

    return EXIT_SUCCESS;
}
