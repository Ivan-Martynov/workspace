#include "mrvn_string_helper.h"

#include <string.h>
#include <malloc.h>

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
    char** restrict const target_ptr, const char* restrict const source)
{
    const size_t source_len = strlen(source);

    const size_t byte_size = source_len * sizeof(*source);
    *target_ptr = malloc(byte_size);
    if (*target_ptr)
    {
        memcpy(*target_ptr, source, byte_size);
        (*target_ptr)[source_len] = '\0';
    }
}
