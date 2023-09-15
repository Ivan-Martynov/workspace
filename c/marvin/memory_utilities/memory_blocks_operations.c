#include "memory_blocks_operations.h"

#include <string.h> // for memcpy

/**
 * @brief Check if two blocks of memory are equal.
 * 
 * @param first_ptr First pointer.
 * @param second_ptr Second pointer.
 * @param block_size Size of memory block.
 * 
 * @return true Blocks are equal.
 * @return false Blocks are not equal.
 */
bool mrvn_blocks_equal(const void* const first_ptr,
    const void* const second_ptr, const size_t block_size)
{
    return memcmp(first_ptr, second_ptr, block_size) == 0;
}

// Swap two blocks of memory byte by byte as unsigned char values.
static void mrvn_swap_voids_v1(
    void* const first_ptr, void* const second_ptr, const size_t block_size)
{
    // Using unsigned char pointers to traverse data byte by byte.
    unsigned char* const a_ptr = first_ptr;
    unsigned char* const b_ptr = second_ptr;

    // Swap each byte of memory blocks.
    for (size_t i = 0; i < block_size; ++i)
    {
        const unsigned char temp = a_ptr[i];
        a_ptr[i] = b_ptr[i];
        b_ptr[i] = temp;
    }
}

#if 0
// Swap two blocks of memory using vla and memcpy.
static void mrvn_swap_voids_v3(
    void* const first_ptr, void* const second_ptr, const size_t block_size)
{
    // Convert void pointers to pointers to unsigned char to get one byte data.
    unsigned char temp[block_size];
    memcpy(temp, first_ptr, block_size);
    memcpy(first_ptr, second_ptr, block_size);
    memcpy(second_ptr, temp, block_size);
}
#endif

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
    mrvn_swap_voids_v1(first_ptr, second_ptr, block_size);
}
