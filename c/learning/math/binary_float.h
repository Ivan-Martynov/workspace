#ifndef _H_BINARY_FLOAT_H_
#define _H_BINARY_FLOAT_H_

#include <stdio.h>

float float_from_bits(const uint32_t);
double double_from_bits(const uint64_t);

uint32_t sign_bit_float(const float);
uint32_t exponent_bits_float(const float);
uint32_t significand_bits_float(const float);

uint64_t sign_bit_double(const double);
uint64_t exponent_bits_double(const double);
uint64_t significand_bits_double(const double);

int print_binary_float(FILE* const, const float);
int print_binary_double(FILE* const, const double);

void show_bits_float(FILE* const, const float);
void show_bits_double(FILE* const, const double);

#endif //_H_BINARY_FLOAT_H_
