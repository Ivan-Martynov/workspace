#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <wchar.h>
#include <stdbool.h>

bool nullify_char(char* line, const char ch);

bool nullify_wchar(wchar_t* const line, const wchar_t ch);

void allocate_and_copy(char** restrict dest, const char* restrict const src);

void wallocate_and_copy(wchar_t** restrict dest, const wchar_t* restrict const src);

void reallocate_and_cat(char** restrict dest, const char* restrict const src);

void wreallocate_and_cat(wchar_t** restrict dest, const wchar_t* restrict const src);

#endif