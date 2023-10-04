#include "binary_float.h"

#include <assert.h>
#include <stdint.h>

static const uint32_t signficand_width_float = 23U;
static const uint32_t signficand_mask_float = 0x007FFFFFU;
static const uint32_t exponent_bias_float = 127U;
static const uint32_t exponent_mask_float = 0xFFU;

static const uint64_t signficand_width_double = 52ULL;
static const uint64_t signficand_mask_double = 0x000FFFFFFFFFFFFFULL;
static const uint64_t exponent_bias_double = 1023ULL;
static const uint64_t exponent_mask_double = 0x7FFULL;

inline float float_from_bits(const uint32_t bits)
{
    return *(const float* const)(&bits);
}

inline double double_from_bits(const uint64_t bits)
{
    return *(const double* const)(&bits);
}

inline uint32_t sign_bit_float(const float value)
{
    const uint32_t* const bit_int_ptr = (const uint32_t* const)(&value);
    return *bit_int_ptr >> 31;
}

inline uint32_t exponent_bits_float(const float value)
{
    const uint32_t* const bit_int_ptr = (const uint32_t* const)(&value);
    return (*bit_int_ptr >> signficand_width_float) & exponent_mask_float;
}

inline uint32_t significand_bits_float(const float value)
{
    const uint32_t* const bit_int_ptr = (const uint32_t* const)(&value);
    return *bit_int_ptr & signficand_mask_float;
}

inline uint64_t sign_bit_double(const double value)
{
    const uint64_t* const bit_int_ptr = (const uint64_t* const)(&value);
    return *bit_int_ptr >> 63ULL;
}

inline uint64_t exponent_bits_double(const double value)
{
    const uint64_t* const bit_int_ptr = (const uint64_t* const)(&value);
    return (*bit_int_ptr >> signficand_width_double) & exponent_mask_double;
}

inline uint64_t significand_bits_double(const double value)
{
    const uint64_t* const bit_int_ptr = (const uint64_t* const)(&value);
    return *bit_int_ptr & signficand_mask_double;
}

int print_binary_float(FILE* const stream, const float value)
{
    const uint32_t* const bit_int_ptr = (const uint32_t* const)(&value);

    size_t i = 31;
    fprintf(stream, "%d ", (*bit_int_ptr >> i) & 1);

    for (size_t j = 0; j < 8; ++j)
    {
        fprintf(stream, "%d", (*bit_int_ptr >> --i) & 1);
    }

    fprintf(stream, " ");

    while (i--)
    {
        fprintf(stream, "%d", (*bit_int_ptr >> i) & 1);
    }

    return fprintf(stream, "\n");
}

int print_binary_double(FILE* const stream, const double value)
{
    const uint64_t* const bit_int_ptr = (const uint64_t* const)(&value);

    size_t i = 63ULL;
    fprintf(stream, "%lld ", (*bit_int_ptr >> i) & 1ULL);

    for (size_t j = 0; j < 11; ++j)
    {
        fprintf(stream, "%lld", (*bit_int_ptr >> --i) & 1ULL);
    }

    fprintf(stream, " ");

    while (i--)
    {
        fprintf(stream, "%lld", (*bit_int_ptr >> i) & 1ULL);
    }

    return fprintf(stream, "\n");
}

static int print_bits_float(FILE* const stream, const float value)
{
    const uint32_t sign_bit = sign_bit_float(value);
    uint32_t exponent_bits = exponent_bits_float(value);
    const uint32_t signficand_bits = significand_bits_float(value);

    if ((exponent_bits == exponent_mask_float) && signficand_bits)
    {
        return fprintf(stream, "NaN\n");
    }

    if (sign_bit)
    {
        fprintf(stream, "-");
    }

    if (exponent_bits == exponent_mask_float)
    {
        return fprintf(stream, "Inf\n");
    }
    else if (exponent_bits)
    {
        fprintf(stream, "1.");
    }
    else
    {
        fprintf(stream, "0.");
        if (signficand_bits)
        {
            ++exponent_bits;
        }
    }

    for (size_t i = signficand_width_float - 1; i < signficand_width_float; --i)
    {
        fprintf(stream, "%d", (signficand_bits >> i) & 1);
    }

    return (exponent_bits || signficand_bits) ? fprintf(
               stream, " * 2^%d\n", (int)(exponent_bits - exponent_bias_float))
                                            : fprintf(stream, "\n");
}

static int print_bits_double(FILE* const stream, const double value)
{
    const uint64_t sign_bit = sign_bit_double(value);
    uint64_t exponent_bits = exponent_bits_double(value);
    const uint64_t signficand_bits = significand_bits_double(value);

    if ((exponent_bits == exponent_mask_double) && signficand_bits)
    {
        return fprintf(stream, "NaN\n");
    }

    if (sign_bit)
    {
        fprintf(stream, "-");
    }

    if (exponent_bits == exponent_mask_double)
    {
        return fprintf(stream, "Inf\n");
    }
    else if (exponent_bits)
    {
        fprintf(stream, "1.");
    }
    else
    {
        fprintf(stream, "0.");
        if (signficand_bits)
        {
            ++exponent_bits;
        }
    }

    for (size_t i = signficand_width_double - 1; i < signficand_width_double;
         --i)
    {
        fprintf(stream, "%lld", (signficand_bits >> i) & 1ULL);
    }

    return (exponent_bits || signficand_bits) ? fprintf(
               stream, " * 2^%d\n", (int)(exponent_bits - exponent_bias_double))
                                              : fprintf(stream, "\n");
}

void show_bits_float(FILE* const stream, const float value)
{
    //static_assert(sizeof(value) == 4, "Size of float should be 4.");

#if 0
    const uint32_t* const bit_int_ptr = (const uint32_t* const) & value;

    fprintf(stream, "%.64f (%a) => ", value, value);

    size_t i = 31;
    fprintf(stream, "sign = %d", ((*bit_int_ptr >> i) & 1));

    fprintf(stream, ", exponent = ");
    for (size_t j = 0; j < 8; ++j)
    {
        fprintf(stream, "%d", ((*bit_int_ptr >> (--i)) & 1));
    }

    fprintf(stream, ", signficand = ");
    do
    {
        fprintf(stream, "%d", ((*bit_int_ptr >> (--i)) & 1));
    } while (i);

    fprintf(stream, "\n");
#endif

    print_bits_float(stream, value);
}

void show_bits_double(FILE* const stream, const double value)
{
    print_bits_double(stream, value);
}
