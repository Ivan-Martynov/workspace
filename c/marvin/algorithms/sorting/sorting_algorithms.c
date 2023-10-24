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

/**
 * @brief Merge left and right parts into the target array.
 * 
 * @param ptr Pointer to the target array.
 * @param temp_ptr Temporary pointer to keep intermediate results.
 * @param left Index of the left element.
 * @param middle Index of the middle element.
 * @param right Index of the right element.
 * @param block_size Number of bytes per element.
 * @param compare_function Function to compare two elements.
 * @param need_to_swap Function to check if elements are to be swapped
 * 
 * @version 0.1
 * 
 * @date 2023-09-06
 */
static void mrvn_merge_parts_with_order_impl(unsigned char* const ptr,
    unsigned char* const temp_ptr, const size_t left, const size_t middle,
    const size_t right, const size_t block_size,
    int compare_function(const void* const, const void* const),
    local_check_function_type need_to_swap)
{
    // Point to the left element in memory.
    size_t i = block_size * left;
    // Block size for elements in the left part.
    const size_t left_block_size = block_size * (middle - left + 1);
    if (left_block_size)
    {
        memcpy(temp_ptr + i, ptr + i, left_block_size);
    }

    // Point to the element next after the middle element in memory.
    size_t j = block_size * (middle + 1);
    // Block size for elements in the right part.
    const size_t right_block_size = block_size * (right - middle);
    if (right_block_size)
    {
        memcpy(temp_ptr + j, ptr + j, right_block_size);
    }

    size_t k = i;
    const size_t left_stop = block_size * middle;
    const size_t right_stop = block_size * right;
    // Looping through left and right parts simultaneiously, choosing an element
    // to put onto the left of the other using the comparison function. The main
    // sorting part.
    while ((i <= left_stop) && (j <= right_stop))
    {
        const unsigned char* const prev_ptr = temp_ptr + i;
        const unsigned char* const next_ptr = temp_ptr + j;

        // If the left element is 'larger', then put it to the right part and
        // the right element is moved to the left part.
        if (need_to_swap(compare_function, prev_ptr, next_ptr))
        {
            // The right element is 'smaller', thus put it into the target array
            // and move the index onto the next element in memory.
            memcpy(ptr + k, next_ptr, block_size);
            j += block_size;
        }
        else
        {
            // The left element is 'smaller', thus put it into the target array
            // and move the index onto the next element in memory.
            memcpy(ptr + k, prev_ptr, block_size);
            i += block_size;
        }
        // Move the index of the target array onto the next element in memory.
        k += block_size;
    }

    // Place remaining elements from the left part into the target array. If
    // there are still elements, then they are 'smaller'.
    while (i <= left_stop)
    {
        memcpy(ptr + k, temp_ptr + i, block_size);
        i += block_size;
        k += block_size;
    }

    // Place remaining elements from the right part into the target array. If
    // there are still elements, then they are 'larger'.
    while (j <= right_stop)
    {
        memcpy(ptr + k, temp_ptr + j, block_size);
        j += block_size;
        k += block_size;
    }
}

/**
 * @brief Merge sort an array, using temporary pointer to keep intermediate
 * results.
 *
 * @param ptr Target pointer.
 * @param temp_ptr Temporary pointer.
 * 
 * @param left Index of the left element.
 * @param right Index of the right element.
 * @param block_size Number of bytes per element.
 * @param compare_function Function to compare two elements.
 * @param need_to_swap Function to check if elements are to be swapped
 *
 * @version 0.1
 *
 * @date 2023-09-06
 */
static void mrvn_merge_sort_with_order_impl(unsigned char* const ptr,
    unsigned char* const temp_ptr, const size_t left, const size_t right,
    const size_t block_size,
    int compare_function(const void* const, const void* const),
    local_check_function_type need_to_swap)
{
    if (left < right)
    {
        const size_t middle = left + ((right - left) >> 1);

        // Dividing the array into two parts.
        mrvn_merge_sort_with_order_impl(ptr, temp_ptr, left, middle, block_size,
            compare_function, need_to_swap);
        mrvn_merge_sort_with_order_impl(ptr, temp_ptr, middle + 1, right,
            block_size, compare_function, need_to_swap);

        // Merging (conquering) the array.
        mrvn_merge_parts_with_order_impl(ptr, temp_ptr, left, middle, right,
            block_size, compare_function, need_to_swap);
    }
}

/**
 * @brief Sort an array using the merge algorithm.
 * 
 * @param ptr Pointer to target array.
 * @param element_count Number of elements in the array.
 * @param block_size Size of each element.
 * @param compare_function Function to compare two elements.
 * @param sorting_order Sorting order.
 * 
 * @version 0.1
 * 
 * @date 2023-09-06
 */
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

    // Temporary array to put elements in for merging.
    unsigned char* const temp_ptr = malloc(element_count * block_size);

    if (!temp_ptr)
    {
        return;
    }

    mrvn_merge_sort_with_order_impl(ptr, temp_ptr, 0, element_count - 1,
        block_size, compare_function, need_to_swap);

    free(temp_ptr);
}

/**
 * @brief Sort an array using the merge algorithm.
 * 
 * @param ptr Pointer to target array.
 * @param element_count Number of elements in the array.
 * @param block_size Size of each element.
 * @param compare_function Function to compare two elements.
 * 
 * @version 0.1
 * 
 * @date 2023-09-06
 */
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
void mrvn_bubble_sort_with_order(void* const ptr, size_t element_count,
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
void mrvn_bubble_sort(void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const))
{
    mrvn_bubble_sort_with_order(
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
    if (!ptr)
    {
        return true;
    }

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
bool mrvn_is_array_sorted(const void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const))
{
    return mrvn_is_array_sorted_with_order(
        ptr, element_count, block_size, compare_function, ASCENDING_STRICT);
}
