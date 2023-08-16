#ifndef _SORTING_ALGOS_H_
#define _SORTING_ALGOS_H_

#include <stdlib.h>

/*
** Utils
*/

void print_array_d(const double[static 1], const size_t);
void print_array_general(
    const void*, const size_t, const size_t, void (*)(const void* const));

bool is_sorted_d(const double[static 1], const size_t);
bool is_sorted_general(
    void*, const size_t, const size_t, int (*)(const void*, const void*));

/*
** Bubble sorting
*/
void bubble_sort_d(double[static 1], size_t);
void bubble_sort_general(
    void*, size_t, const size_t, int (*)(const void*, const void*));

/*
** Merge sorting
*/
void merge_sort_d(double[static 1], const size_t, const size_t);

/*
** Quicksort sorting
*/
void quick_sort_d(double[static 1], const size_t left, const size_t right);

#endif