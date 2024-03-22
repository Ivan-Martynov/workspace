#ifndef _H_MRVN_STRING_HELPER_H_
#define _H_MRVN_STRING_HELPER_H_

#include <stdbool.h>

void mrvn_allocate_and_copy_char(
    char** restrict const, const char* restrict const);

void mrvn_reallocate_if_needed_and_copy_char(
    char** restrict const, const char* restrict const);

bool mrvn_null_or_empty(const char* const);

bool mrvn_null_empty_or_spaces(const char* const);

void mrvn_replace_all_occurrences(char[static 1], const char, const char);

int mrvn_compare_case_insensitive(
    const char[restrict static 1], const char[restrict static 1]);
bool mrvn_equals_case_insensitive(
    const char[restrict static 1], const char[restrict static 1]);

void mrvn_trim_string_from_left_with(
    char[restrict static 1], const char[restrict static 1]);
void mrvn_trim_string_from_right_with(
    char[restrict static 1], const char[restrict static 1]);
void mrvn_trim_string_with(
    char[restrict static 1], const char[restrict static 1]);

#endif //_H_MRVN_STRING_HELPER_H_
