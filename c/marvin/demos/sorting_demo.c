#include <stdio.h>
#include <stdlib.h>

#include "../algorithms/sorting/sorting_algorithms.h"

static void print_size_t_array(
    const size_t element_count, size_t arr[static element_count])
{
    for (size_t i = 0; i < element_count; ++i)
    {
        printf("%zu ", arr[i]);
    }
    printf("\n");
}

static int compare_size_t(
    const void* const first_ptr, const void* const second_ptr)
{
    return *(const size_t*)first_ptr - *(const size_t*)second_ptr;
}

static void test_sorting_bubble_sort(void)
{
    size_t arr[] = {1, 5, 4, 9, 3, 2, 8};
    const size_t n = sizeof(arr) / sizeof(arr[0]);

    print_size_t_array(n, arr);
    mrvn_bubble_sort_generic(arr, n, sizeof(size_t), compare_size_t);
    //mrvn_bubble_sort_generic_with_order(
    //    arr, n, sizeof(size_t), compare_size_t, DESCENDING_NON_STRICT);
    print_size_t_array(n, arr);
}

int main(void)
{
    test_sorting_bubble_sort();

    return EXIT_SUCCESS;
}
