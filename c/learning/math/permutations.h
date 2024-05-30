#ifndef _H_PERMUTATIONS_H_
#define _H_PERMUTATIONS_H_

#include <stddef.h>

void permutations_lexicographic(const size_t n, size_t arr[static n],
    size_t* const* const target);

void permutations_heap(const size_t k, const size_t n,
    size_t arr[static restrict n],
    size_t* const restrict * const restrict target);

void permutations_swap_with_last(const size_t n,
    size_t* const restrict* const restrict target,
    size_t arr[restrict static 1], const size_t current_limit);

#endif //_H_PERMUTATIONS_H_
