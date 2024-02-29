#include "mrvn_string_helper.h"

#include <malloc.h>
#include <string.h>


/**
 * @brief Allocate space for target based on the source's size.
 *
 * @param[in,out] target_ptr Target string.
 * @param[in] source Source string.
 *
 * @version 0.1
 *
 * @date 2024-02-27
 */
void mrvn_allocate_and_copy_char(
    char** restrict const target_ptr, const char* restrict const source_ptr)
{
    const size_t source_len = strlen(source_ptr);
    const size_t byte_size = (source_len + 1) * sizeof(*source_ptr);
#if 0
    if (*target_ptr)
    {
        *target_ptr = realloc(*target_ptr, byte_size);
    }
    else
    {
        *target_ptr = malloc(byte_size);
    }
#else
    *target_ptr = malloc(byte_size);
#endif
    if (*target_ptr)
    {
        memcpy(*target_ptr, source_ptr, byte_size);
    }
}

/**
 * @brief Reallocate target string if it is shorter than source and copy the
 * source.
 *
 * @param target_ptr Target string.
 * @param source_ptr Source string.
 *
 * @version 0.1
 *
 * @date 2024-02-28
 */
void mrvn_reallocate_if_needed_and_copy_char(
    char** restrict const target_ptr, const char* restrict const source_ptr)
{
    const size_t target_len = strlen(*target_ptr);
    const size_t source_len = strlen(source_ptr);
    const size_t byte_size = (source_len + 1) * sizeof(*source_ptr);

    if (target_len < source_len)
    {
        *target_ptr = realloc(*target_ptr, byte_size);
    }

    if (*target_ptr)
    {
        memcpy(*target_ptr, source_ptr, byte_size);
    }
}
