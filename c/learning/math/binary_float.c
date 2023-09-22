#include "binary_float.h"

#include <assert.h>
#include <stdint.h>

#if 0
typedef unsigned int bit_type ;
#else
typedef uint32_t bit_type;
#endif

static const bit_type mantissa_width = 23;
static const bit_type mantissa_mask = 0x007FFFFFu;
static const bit_type exponent_bias = 127;
static const bit_type exponent_mask = 0xFFu;

static bit_type get_sign_bit(const float value)
{
    const bit_type* const bit_int = (const bit_type* const)(&value);
    return *bit_int >> 31;
}

static bit_type get_exponent_bits(const float value)
{
    const bit_type* const bit_int = (const bit_type* const)(&value);
    return (*bit_int >> mantissa_width) & exponent_mask;
}

static bit_type get_mantissa_bits(const float value)
{
    const bit_type* const bit_int = (const bit_type* const)(&value);
    return *bit_int & mantissa_mask;
}

static int print_bits(FILE* stream, const float value)
{
    const bit_type sign_bit = get_sign_bit(value);
    bit_type exponent_bits = get_exponent_bits(value);
    const bit_type mantissa_bits = get_mantissa_bits(value);

    if (sign_bit)
    {
        fprintf(stream, "-");
    }

    if (exponent_bits > exponent_bias << 1)
    {
        return fprintf(stream, mantissa_bits ? "NaN\n" : "Inf\n");
    }
    else if (exponent_bits == 0)
    {
        fprintf(stream, "0.");
        if (mantissa_bits)
        {
            ++exponent_bits;
        }
    }
    else
    {
        fprintf(stream, "1.");
    }

    for (size_t i = mantissa_width - 1; i < mantissa_width; --i)
    {
        fprintf(stream, "%d", (mantissa_bits >> i) & 1);
    }

    return (exponent_bits || mantissa_bits) ? fprintf(
               stream, " * 2^%d\n", (int)(exponent_bits - exponent_bias))
                                            : fprintf(stream, "\n");
}

void show_float_bits(FILE* const stream, const float value)
{
    //static_assert(sizeof(value) == 4, "Size of float should be 4.");

#if 0
    const bit_type* const bit_int = (const bit_type* const) & value;

    fprintf(stream, "%.64f (%a) => ", value, value);

    size_t i = 31;
    fprintf(stream, "sign = %d", ((*bit_int >> i) & 1));

    fprintf(stream, ", exponent = ");
    for (size_t j = 0; j < 8; ++j)
    {
        fprintf(stream, "%d", ((*bit_int >> (--i)) & 1));
    }

    fprintf(stream, ", mantissa = ");
    do
    {
        fprintf(stream, "%d", ((*bit_int >> (--i)) & 1));
    } while (i);

    fprintf(stream, "\n");
#endif

    print_bits(stream, value);
}
