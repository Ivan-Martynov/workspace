#ifndef _H_BINARY_FLOAT_H_
#define _H_BINARY_FLOAT_H_

#include <stdio.h>

float float_from_bits(const uint32_t);

uint32_t float_sign_bit(const float);
uint32_t float_exponent_bits(const float);
uint32_t float_signficand_bits(const float);

int print_binary(FILE*, const float);

void show_float_bits(FILE* const stream, const float);

#endif //_H_BINARY_FLOAT_H_
