#ifndef _H_MRVN_MEMORY_BLOCK_OPERATIONS_H_
#define _H_MRVN_MEMORY_BLOCK_OPERATIONS_H_

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Check if two blocks of memory are equal.
 * 
 */
bool blocks_equal(const void* const, const void* const, const size_t);

/**
 * @brief Swap two blocks of memory with equal size.
 * 
 */
void mrvn_swap_voids(void* const, void* const, const size_t);

#endif // _H_MRVN_MEMORY_BLOCK_OPERATIONS_H_
