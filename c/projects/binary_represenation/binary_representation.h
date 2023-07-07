#ifndef BINARY_REPRESENTATION_H
#define BINARY_REPRESENTATION_H

#include <stdbool.h>
#include <stddef.h>

#define BITS_IN_BYTE 8
#define CHUNK_COUNT sizeof(size_t)
#define BIT_WIDTH (CHUNK_COUNT * BITS_IN_BYTE)

void number_to_array(bool[static BIT_WIDTH], long long);

long long array_to_number(const bool[static BIT_WIDTH]);

void array_to_string_representation(
    char[static BIT_WIDTH + CHUNK_COUNT - 1], const bool[static BIT_WIDTH]);

void binary_sum(bool[static BIT_WIDTH], const bool[static BIT_WIDTH],
    const bool[static BIT_WIDTH]);

#endif // BINARY_REPRESENTATION_H