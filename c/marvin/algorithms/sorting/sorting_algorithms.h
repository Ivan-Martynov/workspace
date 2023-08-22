#ifndef _MRVN_SORTING_ALGORITHMS_H_
#define _MRVN_SORTING_ALGORITHMS_H_

#include <stddef.h>

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

/**
 * @brief Sort an array of data using bubble sort algorithm.
 * 
 */
void mrvn_bubble_sort_generic(void*, size_t, const size_t,
    int (*)(const void* const, const void* const));

/**
 * @brief Sort an array of data using bubble sort algorithm with specified
 * sorting order.
 *
 */
void mrvn_bubble_sort_generic_with_order(void*, size_t, const size_t,
    int (*)(const void* const, const void* const),
    const enum mrvn_sorting_order);

#endif // _MRVN_SORTING_ALGORITHMS_H_
