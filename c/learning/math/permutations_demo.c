#include "permutations.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void print_result(const size_t row_count, const size_t column_count,
    size_t* const* const result)
{
    for (size_t i = 0; i < row_count; ++i)
    {
        for (size_t j = 0; j < column_count; ++j)
        {
            printf("%zu ", result[i][j]);
        }
        printf("\n");
    }
}

static size_t factorial_iterative(size_t n)
{
    size_t result = 1;

    for (; n > 1; --n)
    {
        result *= n;
    }

    return result;
}

static void test_lexicographic(const size_t n)
{
    // Initialize arrays.
    size_t* arr = malloc(n * sizeof(*arr));
    if (!arr)
    {
        fprintf(stderr, "Failed to allocate memory for %zu elements\n", n);
        return;
    }

    for (size_t i = 0; i < n; ++i)
    {
        arr[i] = i + 1;
    }

    const size_t perm_count = factorial_iterative(n);

    size_t** const result = malloc(perm_count * sizeof(*result));
    if (!result)
    {
        free(arr);
        fprintf(stderr, "Failed to allocate memory for %zu elements\n",
            perm_count);
        return;
    }

    for (size_t i = 0; i < perm_count; ++i)
    {
        result[i] = malloc(n * sizeof(*result[i]));
        if (!result[i])
        {
            for (size_t j = 0; j < i; ++j)
            {
                free(result[i]);
            }
            free(result);
            fprintf(stderr, "Failed to allocate memory for %zu elements\n", n);
            return;
        }
    }

    //struct timespec start_spec;
    //struct timespec end_spec;

    //timespec_get(&start_spec, TIME_UTC);
    permutations_lexicographic(n, arr, result);
    //print_result(perm_count, n, result);
    //printf("Found %zu permutations\n", perm_count);
    permutations_heap(n, n, arr, result);

    //timespec_get(&end_spec, TIME_UTC);
    //const double d1 = gettime_diff_milliseconds(&start_spec, &end_spec);

    print_result(perm_count, n, result);
    printf("Found %zu permutations\n", perm_count);

    // Free arrays.
    for (size_t i = 0; i < perm_count; ++i)
    {
        free(result[i]);
    }
    free(result);
    free(arr);
}

void test_swap_with_last(const size_t n)
{
    // Initialize arrays.
    size_t* arr = malloc(n * sizeof(*arr));
    if (!arr)
    {
        fprintf(stderr, "Failed to allocate memory for %zu elements\n", n);
        return;
    }

    for (size_t i = 0; i < n; ++i)
    {
        arr[i] = i + 1;
    }

    const size_t perm_count = factorial_iterative(n);

    size_t** result = malloc(perm_count * sizeof(*result));
    if (!result)
    {
        free(arr);
        fprintf(stderr, "Failed to allocate memory for %zu elements\n",
            perm_count);
        return;
    }

    for (size_t i = 0; i < perm_count; ++i)
    {
        result[i] = malloc(n * sizeof(*result[i]));
        if (!result[i])
        {
            for (size_t j = 0; j < i; ++j)
            {
                free(result[i]);
            }
            free(result);
            fprintf(stderr, "Failed to allocate memory for %zu elements\n", n);
            return;
        }
    }

    //struct timespec start_spec;
    //struct timespec end_spec;

    //timespec_get(&start_spec, TIME_UTC);

    //timespec_get(&end_spec, TIME_UTC);
    //const double d1 = gettime_diff_milliseconds(&start_spec, &end_spec);

    // Calculate permutations.
    permutations_swap_with_last(n, result, arr, n);
    print_result(perm_count, n, result);
    printf("Found %zu permutations\n", perm_count);

    permutations_swap_with_last(n, result, arr, n);

    print_result(perm_count, n, result);
    printf("Found %zu permutations\n", perm_count);

    // Free arrays.
    for (size_t i = 0; i < perm_count; ++i)
    {
        free(result[i]);
    }
    free(result);
    free(arr);
}

int main(const int argc, const char* argv[static 1])
{
    if (argc > 1)
    {
        size_t n = strtoull(argv[1], NULL, 10);
        test_swap_with_last(n);
    }
    else
    {
        test_lexicographic(4);
    }

    return EXIT_SUCCESS;
}
