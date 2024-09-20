#include "SyncsafeIntConverter.h"

namespace Marvin
{

/**
 * @brief Bytes to integer.
 *
 * @param[in] bytes Bytes representing the value.
 * @param[in] size Number of bytes.
 *
 * @return size_t Calculated value.
 */
size_t SyncsafeIntConverter::bytes_to_size_t(
    const char* const bytes, const size_t size)
{
    size_t result {0};

    for (size_t i {0}; i < size; ++i)
    {
        result <<= 8;
        result |= static_cast<unsigned char>(bytes[i]);
    }

    return result;
}

/**
 * @brief Encode value as a syncsafe integer.
 *
 * @param[in] value Value to encode.
 *
 * @return size_t Encoded value.
 */
size_t SyncsafeIntConverter::syncsafe_encode(size_t value)
{
    size_t result {0};

    // In a syncsafe integer the MSB is always zero.
    size_t mask {0x7F}; // 2^7 - 1

    // Check bits parity. Keep going until the mask equals 0x7FFFFFFF.
    while (mask ^ 0x7FFFFFFF)
    {
        // Get bits at the MSB and "bigger" bigs zeroing "smaller" bits.
        result = value & ~mask;

        // Shift by one to zero the previous MSB.
        result <<= 1;

        // Add bits of the original value, except the MSB.
        result |= value & mask;

        // E.g., 2^7 - 1 => 2^7 * 2^8 - 1 => 2^15 - 1 = 32767
        // 0x7F => 0xF0 => 0xF000 => 0x7FFF
        // 0x7FFF => 0xF000 => 0xF00000 => 0x7FFFFF
        // 0x7FFFFF => 0xF00000 => 0xF0000000 => 0x7FFFFFFF
        // Thus the loop has 3 steps.
        mask = ((mask + 1) << 8) - 1;

        value = result;
    }

    return result;
}

static size_t syncsafe_decode_impl1(const size_t value)
{
    // Example 383 (255) = 0000 0001 0111 1111 (0000 0000 1111 1111)
    size_t result {0};

    // Process the number by blocks of 16 bits.
    size_t mask {0x7F000000}; // 0111 1111 0...
    while (mask)
    {
        // Shift the "bigger" bits by one to compensate for the shift happened
        // during encoding.
        result >>= 1;

        // 0000 0001 0111 1111 => 0000 0001 0000 0000 => 1000 0000
        // 0111 1111 => 0111 1111 => 1000 0000 | 0111 1111 => 1111 1111
        // Add original "smaller" bits.
        result |= value & mask;

        // Shift mask to process the next block of bits.
        mask >>= 8;
    }

    return result;
}

[[maybe_unused]]
static size_t syncsafe_decode_impl2(const size_t value)
{
    const size_t a {value & 0xFF};
    const size_t b {(value >> 8) & 0xFF};
    const size_t c {(value >> 16) & 0xFF};
    const size_t d {(value >> 24) & 0xFF};

    size_t result {0x0};
    result = result | a;
    result = result | (b << 7);
    result = result | (c << 14);
    result = result | (d << 21);

    return result;
}

/**
 * @brief Decode value as a syncsafe integer.
 *
 * @param[in] value Value to decode.
 *
 * @return size_t Decoded value.
 */
size_t SyncsafeIntConverter::syncsafe_decode(const size_t value)
{
#if 1
    return syncsafe_decode_impl1(value);
#else
    return syncsafe_decode_impl2(value);
#endif
}

/**
 * @brief Decode value as a syncsafe integer,converting bytes to integer.
 *
 * @param[in] bytes Bytes representing the value.
 * @param[in] size Number of bytes.
 *
 * @return size_t Decoded value.
 */
size_t SyncsafeIntConverter::syncsafe_decode_from_bytes(
    const char* const bytes, const size_t size)
{
    return syncsafe_decode(bytes_to_size_t(bytes, size));
}

} // namespace Marvin
