#include "sorting_algorithms.h"
#include "../../memory_utilities/memory_blocks_operations.h"

#include <stdlib.h> // malloc and friends.
#include <string.h> // memcpy.

/**
 * @brief Type definition for a function to check if elements of an array are to
 * be swapped.
 */
typedef bool (*local_check_function_type)(
    int (*)(const void* const, const void* const), const void* const,
    const void* const);

/**
 * @brief Sort element in the ascending order, strict (that is, not swapping
 * equal elements).
 *
 * @param compare_function Function to compare elements.
 * @param first_ptr Pointer to the first element.
 * @param second_ptr Pointer to the second element.
 *
 * @return true Elements are to be swapped/sorted.
 * @return false Elements are sorted, no need to swap.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
static bool ascend_strict(
    int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) > 0;
}

/**
 * @brief Sort element in the ascending order, non-strict (that is, swapping
 * equal elements).
 *
 * @param compare_function Function to compare elements.
 * @param first_ptr Pointer to the first element.
 * @param second_ptr Pointer to the second element.
 *
 * @return true Elements are to be swapped/sorted.
 * @return false Elements are sorted, no need to swap.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
static bool ascend_non_strict(
    int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) >= 0;
}

/**
 * @brief Sort element in the descending order, strict (that is, not swapping
 * equal elements).
 *
 * @param compare_function Function to compare elements.
 * @param first_ptr Pointer to the first element.
 * @param second_ptr Pointer to the second element.
 *
 * @return true Elements are to be swapped/sorted.
 * @return false Elements are sorted, no need to swap.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
static bool descend_strict(
    int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) < 0;
}

/**
 * @brief Sort element in the descending order, non-strict (that is, swapping
 * equal elements).
 *
 * @param compare_function Function to compare elements.
 * @param first_ptr Pointer to the first element.
 * @param second_ptr Pointer to the second element.
 *
 * @return true Elements are to be swapped/sorted.
 * @return false Elements are sorted, no need to swap.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
static bool descend_non_strict(
    int compare_function(const void* const, const void* const),
    const void* const first_ptr, const void* const second_ptr)
{
    return compare_function(first_ptr, second_ptr) <= 0;
}

/**
 * @brief Choosing function to compare elements using sorting order.
 *
 * @param sorting_order Order to sort elements.
 *
 * @return local_check_function_type Function pointer.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
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

static void mrvn_merge_parts_with_order_impl(void* const ptr, const size_t left,
    const size_t middle, const size_t right, const size_t block_size,
    int compare_function(const void* const, const void* const),
    local_check_function_type need_to_swap)
{
    const size_t first_n = middle - left + 1;
    unsigned char* const first_subarray = malloc(first_n * block_size);

    if (!first_subarray)
    {
        return;
    }

    memcpy(first_subarray, (unsigned char*)ptr + left * block_size,
        block_size * first_n);

    const size_t second_n = right - middle;
    unsigned char* const second_subarray = malloc(second_n * block_size);

    if (!second_subarray)
    {
        return;
    }

    memcpy(second_subarray, (unsigned char*)ptr + block_size * (middle + 1),
        block_size * second_n);

    size_t i = 0;
    size_t j = 0;
    size_t k = left;

    while ((i < first_n) && (j < second_n))
    {
        const void* const prev_ptr = first_subarray + block_size * i;
        const void* const next_ptr = second_subarray + block_size * j;
        if (need_to_swap(compare_function, prev_ptr, next_ptr))
        {
            memcpy((unsigned char*)ptr + block_size * k, next_ptr, block_size);
            ++j;
        }
        else
        {
            memcpy((unsigned char*)ptr + block_size * k, prev_ptr, block_size);
            ++i;
        }
        ++k;
    }

    while (i < first_n)
    {
        memcpy((unsigned char*)ptr + block_size * k,
            first_subarray + block_size * i, block_size);
        ++i;
        ++k;
    }

    while (j < second_n)
    {
        memcpy((unsigned char*)ptr + block_size * k,
            second_subarray + block_size * j, block_size);
        ++j;
        ++k;
    }

    free(first_subarray);
    free(second_subarray);
}

static void mrvn_merge_sort_with_order_impl(void* const ptr, const size_t left,
    const size_t right, const size_t block_size,
    int compare_function(const void* const, const void* const),
    local_check_function_type need_to_swap)
{
    if (left < right)
    {
        const size_t middle = left + ((right - left) >> 1);

        mrvn_merge_sort_with_order_impl(
            ptr, left, middle, block_size, compare_function, need_to_swap);
        mrvn_merge_sort_with_order_impl(
            ptr, middle + 1, right, block_size, compare_function, need_to_swap);

        mrvn_merge_parts_with_order_impl(ptr, left, middle, right, block_size,
            compare_function, need_to_swap);
    }
}

void mrvn_merge_sort_with_order(void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const),
    const enum mrvn_sorting_order sorting_order)
{
    if (!ptr)
    {
        return;
    }

    local_check_function_type need_to_swap = sort_order_function(sorting_order);

    mrvn_merge_sort_with_order_impl(
        ptr, 0, element_count - 1, block_size, compare_function, need_to_swap);
}

void mrvn_merge_sort(void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const))
{
    mrvn_merge_sort_with_order(
        ptr, element_count, block_size, compare_function, ASCENDING_STRICT);
}

/**
 * @brief Sort an array of data using bubble sort algorithm with specified
 * sorting order.
 *
 * @param ptr Pointer to data.
 * @param element_count Number of elements in the array.
 * @param block_size Size of each element.
 * @param compare_function Function to compare two elements.
 * @param sorting_order Sorting order.
 */
void mrvn_bubble_sort_generic_with_order(void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const),
    const enum mrvn_sorting_order sorting_order)
{
    if (!ptr)
    {
        return;
    }

    local_check_function_type need_to_swap = sort_order_function(sorting_order);

    while (true)
    {
        // Flag to track if the array has changed. Used to get out the loop if
        // the array has not been changed.
        bool no_change = true;

        void* prev_ptr = ptr;

        for (size_t i = 1; i < element_count; ++i)
        {
            // Point to the next block block_size bytes forward.
            void* next_ptr = (unsigned char* const)prev_ptr + block_size;

            if (need_to_swap(compare_function, prev_ptr, next_ptr))
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
void mrvn_bubble_sort_generic(void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const))
{
    mrvn_bubble_sort_generic_with_order(
        ptr, element_count, block_size, compare_function, ASCENDING_STRICT);
}

/**
 * @brief Check if array is sorted with specified sorting order.
 *
 * @param ptr Pointer to data.
 * @param element_count Number of elements in the array.
 * @param block_size Size of each element.
 * @param compare_function Function to compare two elements.
 * @param sorting_order Sorting order.
 *
 * @return true Array is sorted.
 * @return false Array is not sorted.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
bool mrvn_is_array_sorted_with_order(const void* const ptr,
    size_t element_count, const size_t block_size,
    int compare_function(const void* const, const void* const),
    const enum mrvn_sorting_order sorting_order)
{
    local_check_function_type need_to_swap = sort_order_function(sorting_order);

    const void* prev_ptr = ptr;

    for (size_t i = 1; i < element_count; ++i)
    {
        // Point to the next block block_size bytes forward.
        const void* next_ptr
            = (const unsigned char* const)prev_ptr + block_size;

        if (need_to_swap(compare_function, prev_ptr, next_ptr))
        {
            return false;
        }

        prev_ptr = next_ptr;
    }

    return true;
}

/**
 * @brief Check if array is sorted.
 *
 * @param ptr Pointer to data.
 * @param element_count Number of elements in the array.
 * @param block_size Size of each element.
 * @param compare_function Function to compare two elements.
 *
 * @return true Array is sorted.
 * @return false Array is not sorted.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
bool mrvn_is_array_sorted_generic(const void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const))
{
    return mrvn_is_array_sorted_with_order(
        ptr, element_count, block_size, compare_function, ASCENDING_STRICT);
}
