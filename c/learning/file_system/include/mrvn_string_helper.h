#ifndef _H_MRVN_STRING_HELPER_H_
#define _H_MRVN_STRING_HELPER_H_

#include <stdbool.h>

void mrvn_allocate_and_copy_char(
    char** restrict const, const char* restrict const);

void mrvn_reallocate_if_needed_and_copy_char(
    char** restrict const, const char* restrict const);

bool mrvn_null_or_empty(const char* const);

bool mrvn_null_empty_or_spaces(const char* const);

#endif //_H_MRVN_STRING_HELPER_H_
