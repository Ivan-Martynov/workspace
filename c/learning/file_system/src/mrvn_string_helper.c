#include "mrvn_string_helper.h"
#include "mrvn_type_constants.h"

#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <errno.h>

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

char* mrvn_allocate_char_from_another(const char* const other)
{
    char* target = mrvn_null_ptr;

    if (other)
    {
        mrvn_allocate_and_copy_char(&target, other);
    }

    return target;
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
    // if (!text)
    // {
    //     return;
    // }

    size_t i = strlen(text) - 1;
    do
    {
        if (text[i] == to_replace)
        {
            text[i] = replacement;
        }
    } while (i--);
}

int mrvn_compare_case_insensitive(const char text[restrict static 1],
    const char text_in_uppercase[restrict static 1])
{
    const unsigned char* lhs = (const unsigned char*)text;
    const unsigned char* rhs = (const unsigned char*)text_in_uppercase;

    while ((*lhs != '\0') && (tolower(*lhs) == tolower(*rhs)))
    {
        ++lhs;
        ++rhs;
    }

    return tolower(*lhs) - tolower(*rhs);
}

bool mrvn_equals_case_insensitive(
    const char lhs[restrict static 1], const char rhs[restrict static 1])
{
    return mrvn_compare_case_insensitive(lhs, rhs) == 0;
}

void mrvn_trim_string_from_left_with(char text[restrict static 1],
    const char characters_to_trim[restrict static 1])
{
    char* start = text;

    while (*text && strchr(characters_to_trim, *text))
    {
        ++text;
    }

    const char* current = text;
    if (start != current)
    {
        while (*text)
        {
            ++text;
        }

        memmove(start, current, text - current + sizeof(*text));
    }
}

void mrvn_trim_string_from_right_with(char text[restrict static 1],
    const char characters_to_trim[restrict static 1])
{
    size_t i = strlen(text);
    text += i;

    while (i-- && strchr(characters_to_trim, *--text))
    {
        *text = '\0';
    }
}

void mrvn_trim_string_with(char text[restrict static 1],
    const char characters_to_trim[restrict static 1])
{
    char* start = text;

    while (*text && strchr(characters_to_trim, *text))
    {
        ++text;
    }

    // If reached the end of the string, the all characters are to be removed.
    if (!*text)
    {
        *start = '\0';
        return;
    }

    // Current cannot be null nor a character to trim.
    const char* current = text;

    // Continue to the end of the string.
    while (*text)
    {
        ++text;
    }

    // Trim the right side of the string.
    while (strchr(characters_to_trim, *--text))
    {
        *text = '\0';
    }

    // Move only if to be trimmed from the left.
    if (start != current)
    {
        memmove(start, current, text - current + 2 * sizeof(*text));
    }
}

/**
 * @brief Calculate length of a null terminated multibyte string.
 * 
 * @param src Null terminated multibyte string.
 * 
 * @return size_t Number of character in the string.
 * 
 * @version 0.1
 * 
 * @date 2024-04-23
 */
size_t mrvn_multibyte_string_length(const char src[static 1])
{
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    size_t len = mbsrtowcs(mrvn_null_ptr, &src, 0, &state);

    // Restore errno on error.
    if (len == (size_t)-1)
    {
        errno = 0;
    }

    return len;
}
