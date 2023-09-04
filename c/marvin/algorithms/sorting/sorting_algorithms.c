#include "sorting_algorithms.h"
#include "../../memory_utilities/memory_blocks_operations.h"

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

    local_check_function_type need_to_swap
        = sort_order_function(sorting_order);

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
