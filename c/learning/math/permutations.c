#include <string.h>
#include <stdbool.h>

/**
 * @brief Swap two array elements defined by their indices.
 * 
 * @param arr Sequence to work on.
 * @param i1 Index of the first element.
 * @param i2 Index of the second element.
 * 
 * @version 0.1
 * 
 * @date 2024-05-30
 */
static void swap_array_values(size_t arr[static 1], const size_t i1,
    const size_t i2)
{
    const size_t temp = arr[i1];
    arr[i1] = arr[i2];
    arr[i2] = temp;
}

/**
 * @brief Reverse part of the array between indices @i1 and @i2.
 * 
 * @param arr Array to work on.
 * @param i1 First index.
 * @param i2 Second index.
 * 
 * @version 0.1
 * 
 * @date 2024-05-30
 */
void reverse_array_in_range(size_t arr[static 1], const size_t i1,
    const size_t i2)
{
    const size_t count = (i2 - i1 + 1) / 2;
    for (size_t i = 0; i < count; ++i)
    {
        swap_array_values(arr, i1 + i, i2 - i);
    }
}

/**
 * @brief Generate permutations in a lexicographic order. The original array is
 * modified, representing the last permutation.
 * 
 * @param n Size of array.
 * @param arr Array of size_t values.
 * @param target Two dimensional array to hold all permutations. Should be
 * preallocated.
 * 
 * @version 0.1
 * 
 * @date 2024-05-30
 */
void permutations_lexicographic(const size_t n, size_t arr[static n],
    size_t* const* const target)
{
    size_t perm_index = 0;
    memcpy(target[perm_index], arr, n * sizeof(*arr));
    do
    {
        // Find largest index k so that a[k] < a[k + 1].
        size_t k = n;
        for (size_t i = 0; i < n - 1; ++i)
        {
            if (arr[i] < arr[i + 1])
            {
                k = i;
            }
        }

        if (k != n)
        {
            size_t m = k + 1;
            // Find largest index m so that a[k] < a[m].
            for (size_t i = k + 2; i < n; ++i)
            {
                if (arr[k] < arr[i])
                {
                    m = i;
                }
            }

            // Swap array values at positions k and m.
            swap_array_values(arr, k, m);

            // Reverse the sequence a[k + 1, n - 1], that is all elements after
            // the kth element.
            reverse_array_in_range(arr, k + 1, n - 1);

            // Copy current permutation into the next row in the collection of
            // all permutations.
            memcpy(target[++perm_index], arr, n * sizeof(*arr));
        }
        else
        {
            // Last permutation was found.
            break;
        }
    } while (true);
}

/**
 * @brief Permutations using Heap's algorithm. The original array is modified.
 * 
 * @param k Current limit for the array size.
 * @param n Total size of the array.
 * @param arr Array to work on.
 * @param target Two dimensional array to keep all the permutations.
 * 
 * @version 0.1
 * 
 * @date 2024-05-30
 */
void permutations_heap(const size_t k, const size_t n,
    size_t arr[static restrict n],
    size_t* const restrict * const restrict target)
{
    static size_t perm_index = 0;

    if (k == 1)
    {
        memcpy(target[perm_index], arr, n * sizeof(*arr));
        ++perm_index;
    }
    else
    {
        if (k == n)
        {
            perm_index = 0;
        }

        permutations_heap(k - 1, n, arr, target);

        const bool is_even = (k % 2) == 0;

        for (size_t i = 0; i < k - 1; ++i)
        {
            swap_array_values(arr, is_even ? i : 0, k - 1);
            permutations_heap(k - 1, n, arr, target);
        }
    }
}

void permutations_swap_with_last(const size_t n,
    size_t* const restrict* const restrict target,
    size_t arr[restrict static 1], const size_t current_limit)
{
    static size_t k = 0;
    if (current_limit == 1)
    {
        memcpy(target[k], arr, n * sizeof(arr[0]));
        ++k;
    }
    else
    {
        if (current_limit == n)
        {
            k = 0;
        }
        for (size_t i = 0; i < current_limit; ++i)
        {
            // Swap to perform a basic permutation.
            swap_array_values(arr, i, current_limit - 1);

            permutations_swap_with_last(n, target, arr, current_limit - 1);

            // Swap back to restore.
            swap_array_values(arr, i, current_limit - 1);
        }
    }
}

