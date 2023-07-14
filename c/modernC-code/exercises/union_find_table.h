#ifndef UNION_FIND_TABLE_H
#define UNION_FIND_TABLE_H

#include <stdlib.h>
#include <stdint.h>

#define ROOT_VALUE SIZE_MAX

void initialize(const size_t count, size_t[static count]);

size_t find_root(size_t[static 1], const size_t);

//size_t find_and_replace(size_t[static 1], const size_t);

size_t compress(size_t[static 1], const size_t);

void print_parent(const size_t count, size_t[static count]);

#endif // UNION_FIND_TABLE_H
