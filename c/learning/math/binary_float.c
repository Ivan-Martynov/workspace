#include "binary_float.h"

#include <assert.h>
#include <stdint.h>

static const uint32_t signficand_width = 23;
static const uint32_t signficand_mask = 0x007FFFFFu;
static const uint32_t exponent_bias = 127;
static const uint32_t exponent_mask = 0xFFu;

static uint32_t get_sign_bit(const float value)
{
    const uint32_t* const bit_int = (const uint32_t* const)(&value);
    return *bit_int >> 31;
}

static uint32_t get_exponent_bits(const float value)
{
    const uint32_t* const bit_int = (const uint32_t* const)(&value);
    return (*bit_int >> signficand_width) & exponent_mask;
}

static uint32_t get_signficand_bits(const float value)
{
    const uint32_t* const bit_int = (const uint32_t* const)(&value);
    return *bit_int & signficand_mask;
}

static int print_bits(FILE* stream, const float value)
{
    const uint32_t sign_bit = get_sign_bit(value);
    uint32_t exponent_bits = get_exponent_bits(value);
    const uint32_t signficand_bits = get_signficand_bits(value);

    if ((exponent_bits == exponent_mask) && signficand_bits)
    {
        return fprintf(stream, "NaN\n");
    }

    if (sign_bit)
    {
        fprintf(stream, "-");
    }

    if (exponent_bits == exponent_mask)
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

    for (size_t i = signficand_width - 1; i < signficand_width; --i)
    {
        fprintf(stream, "%d", (signficand_bits >> i) & 1);
    }

    return (exponent_bits || signficand_bits) ? fprintf(
               stream, " * 2^%d\n", (int)(exponent_bits - exponent_bias))
                                            : fprintf(stream, "\n");
}

void show_float_bits(FILE* const stream, const float value)
{
    //static_assert(sizeof(value) == 4, "Size of float should be 4.");

#if 0
    const uint32_t* const bit_int = (const uint32_t* const) & value;

    fprintf(stream, "%.64f (%a) => ", value, value);

    size_t i = 31;
    fprintf(stream, "sign = %d", ((*bit_int >> i) & 1));

    fprintf(stream, ", exponent = ");
    for (size_t j = 0; j < 8; ++j)
    {
        fprintf(stream, "%d", ((*bit_int >> (--i)) & 1));
    }

    fprintf(stream, ", signficand = ");
    do
    {
        fprintf(stream, "%d", ((*bit_int >> (--i)) & 1));
    } while (i);

    fprintf(stream, "\n");
#endif

    print_bits(stream, value);
}
