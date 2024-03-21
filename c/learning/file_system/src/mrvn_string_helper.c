#include "mrvn_string_helper.h"

#include <malloc.h>
#include <string.h>
#include <ctype.h>

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

/**
 * @brief Check whether string is a null pointer or empty.
 * 
 * @param text String to check.
 * 
 * @return true If string is a null pointer or empty.
 * @return false String is not null and has characters.
 * 
 * @version 0.1
 * 
 * @date 2024-03-15
 */
bool mrvn_null_or_empty(const char* const text)
{
    return !text || (strlen(text) == 0);
}

/**
 * @brief
 *
 * @param text
 *
 * @return true
 * @return false
 *
 * @version 0.1
 *
 * @date 2024-03-15
 */
bool mrvn_null_empty_or_spaces(const char* const text)
{
    if (!text)
    {
        return true;
    }

    const size_t len = strlen(text);

    for (size_t i = 0; i < len; ++i)
    {
        if (!isspace((int)text[i]))
        {
            return false;
        }
    }

    return true;
}

void mrvn_replace_all_occurrences(
    char text[static 1], const char to_replace, const char replacement)
{
    if (!text)
    {
        return;
    }

    size_t i = strlen(text) - 1;
    do
    {
        if (text[i] == to_replace)
        {
            text[i] = replacement;
        }
    } while (i--);
}
