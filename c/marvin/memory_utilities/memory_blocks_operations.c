#include "memory_blocks_operations.h"

/**
 * @brief Swap two blocks of memory with equal size.
 *
 * @param first_ptr Pointer to the first block.
 * @param second_ptr Pointer to the second block.
 * @param block_size Size of the given memory blocks.
 */
void mrvn_swap_voids(
    void* const first_ptr, void* const second_ptr, const size_t block_size)
{
    // Convert void pointers to pointers to unsigned char to get one byte data.
    unsigned char* const a_ptr = (unsigned char* const)first_ptr;
    unsigned char* const b_ptr = (unsigned char* const)second_ptr;

    // Swap each byte of memory blocks.
    for (size_t i = 0; i < block_size; ++i)
    {
        const unsigned char temp = a_ptr[i];
        a_ptr[i] = b_ptr[i];
        b_ptr[i] = temp;
    }
}
