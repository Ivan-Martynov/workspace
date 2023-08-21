#include "sorting_algorithms.h"
#include "../../memory_utilities/memory_blocks_operations.h"

#include <stdbool.h>

typedef int (*compare_function_type)(const void* const, const void* const);
typedef bool (*local_check_function_type)(
    const compare_function_type, const void* const, const void* const);

static bool ascend_strict(
    const int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) > 0;
}

static bool ascend_non_strict(
    const int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) >= 0;
}

static bool descend_strict(
    const int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) < 0;
}

static bool descend_non_strict(
    const int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) <= 0;
}

static local_check_function_type sort_order_function(
    const enum mrvn_sorting_order sorting_order)
{
    switch (sorting_order)
    {
        case ASCENDING_STRICT:
            return ascend_strict;

        case ASCENDING_NON_STRICT:
            return ascend_non_strict;

        case DESCENDING_STRICT:
            return descend_strict;

        case DESCENDING_NON_STRICT:
            return descend_non_strict;

        default:
            return ascend_strict;
    }
}

void mrvn_bubble_sort_generic_with_order(void* ptr, size_t element_count,
    const size_t block_size, const compare_function_type compare_function,
    const enum mrvn_sorting_order sorting_order)
{
    const local_check_function_type check_to_swap
        = sort_order_function(sorting_order);

    while (true)
    {
        // Flag to track if the array has changed. Used to get out the loop if
        // the array has not been changed.
        bool no_change = true;

        void* prev_ptr = ptr;

        for (size_t i = 1; i < element_count; ++i)
        {
            void* next_ptr = prev_ptr + block_size;

            if (check_to_swap(compare_function, prev_ptr, next_ptr))
            {
                mrvn_swap_voids(prev_ptr, next_ptr, block_size);
                no_change = false;
            }

            prev_ptr = next_ptr;
        }

        if (no_change)
        {
            break;
        }

        // Since the last element is in its place, then there is no need to
        // check for the last position.
        --element_count;
    }
}

/**
 * @brief Sort an array of data using bubble sort algorithm.
 * 
 * @param ptr Pointer to data.
 * @param element_count Number of elements in the array.
 * @param block_size Size of each element.
 * @param compare_function Function to compare two elements.
 */
void mrvn_bubble_sort_generic(void* ptr, size_t element_count,
    const size_t block_size,
    const int compare_function(const void* const, const void* const))
{
    while (true)
    {
        // Flag to track if the array has changed. Used to get out the loop if
        // the array has not been changed.
        bool no_change = true;

        void* prev_ptr = ptr;

        for (size_t i = 1; i < element_count; ++i)
        {
            void* next_ptr = prev_ptr + block_size;

            if (compare_function(prev_ptr, next_ptr) > 0)
            {
                mrvn_swap_voids(prev_ptr, next_ptr, block_size);
                no_change = false;
            }

            prev_ptr = next_ptr;
        }

        if (no_change)
        {
            break;
        }

        // Since the last element is in its place, then there is no need to
        // check for the last position.
        --element_count;
    }
}
