#ifndef _H_MRVN_SORTING_ALGORITHMS_H_
#define _H_MRVN_SORTING_ALGORITHMS_H_

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Sorting order.
 * 
 */
enum mrvn_sorting_order
{
    ASCENDING_STRICT,
    ASCENDING_NON_STRICT,
    DESCENDING_STRICT,
    DESCENDING_NON_STRICT,
};

void mrvn_merge_sort_with_order(void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const),
    const enum mrvn_sorting_order sorting_order);

void mrvn_merge_sort(void* const ptr, size_t element_count,
    const size_t block_size,
    int compare_function(const void* const, const void* const));

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
    const enum mrvn_sorting_order sorting_order);

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
    int compare_function(const void* const, const void* const));

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
    const enum mrvn_sorting_order sorting_order);

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
    int compare_function(const void* const, const void* const));

#endif // _H_MRVN_SORTING_ALGORITHMS_H_
