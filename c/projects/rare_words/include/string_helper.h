#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <wchar.h>
#include <stdbool.h>

bool nullify_char_wide(wchar_t* const line, const wchar_t ch);

bool allocate_and_copy_wide(wchar_t** restrict dest, const wchar_t* restrict const src);

bool reallocate_and_cat_wide(wchar_t** restrict dest, const wchar_t* restrict const src);

#endif