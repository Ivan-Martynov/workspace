#include "binary_representation.h"

/**
 * @brief Fill a bool array with values for the binary representation of the
 * given number.
 *
 * @param target_array Array to fill.
 * @param number Number to process.
 *
 * @version 0.1
 *
 * @author Ivan Martynov
 *
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 */
void number_to_array(bool target_array[static BIT_WIDTH], long long number)
{
    const size_t last_index = BIT_WIDTH - 1;
    for (size_t i = 0; i < BIT_WIDTH; ++i)
    {
        // Shift the current index to represent the power of two.
        const size_t value = 1ULL << i;
        // Inserting values starting from the end of the array.
        target_array[last_index - i] = value & number;
    }
}

/**
 * @brief Convert a binary representation to a number.
 * 
 * @param bits_array Binary representation via array of boolean values.
 * 
 * @return long long The corresponding number.
 * 
 * @version 0.1
 * 
 * @author Ivan Martynov
 * 
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 */
long long array_to_number(const bool bits_array[static BIT_WIDTH])
{
    // Flag for negative numbers.
    const bool sign_flag = bits_array[0];

    // Traverse through all values equal to the sign_flag until found a distinct
    // value or reached the end.
    size_t index = 1;
    while ((bits_array[index] == sign_flag) && (index < BIT_WIDTH))
    {
        ++index;
    }

    if (index == BIT_WIDTH)
    {
        return sign_flag ? -1 : 0;
    }

    long long result = 1;
    for (++index; index < BIT_WIDTH; ++index)
    {
#if 1
        result <<= 1;
        if (bits_array[index] != sign_flag)
        {
            ++result;
        }
#else
        result = (result << 1) + (bits_array[index] != sign_flag);
#endif
    }

    return sign_flag ? -result - 1 : result;
}

/**
 * @brief Make a string representation of a binary array, adding spaces to group
 * the values.
 *
 * @param target_string Target string.
 * @param bits_array Source array.
 *
 * @version 0.1
 *
 * @author Ivan Martynov
 *
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 */
void array_to_string_representation(
    char target_string[static BIT_WIDTH + CHUNK_COUNT - 1],
    const bool bits_array[static BIT_WIDTH])
{
    size_t k = 0;
    for (size_t i = 0; i < BIT_WIDTH; i += BITS_IN_BYTE)
    {
        const size_t stop = i + BITS_IN_BYTE;
        for (size_t j = i; j < stop; ++j)
        {
            target_string[k++] = bits_array[j] ? '1' : '0';
        }
        target_string[k++] = ' ';
    }
    target_string[k] = '\0';
}

/**
 * @brief Perform binary addition between two arrays, placing the result in the
 * target array.
 *
 * @param target_array Target array.
 * @param first_array First array.
 * @param second_array Second array.
 *
 * @version 0.1
 *
 * @author Ivan Martynov
 *
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 */
void binary_sum(bool target_array[static BIT_WIDTH],
    const bool first_array[static BIT_WIDTH],
    const bool second_array[static BIT_WIDTH])
{
    bool accumulate = false;
    for (size_t i = BIT_WIDTH - 1; i < BIT_WIDTH; --i)
    {
        const bool first_value = first_array[i];
        const bool second_value = second_array[i];

        if (accumulate)
        {
            if (!(first_value || second_value))
            {
                target_array[i] = true;
                accumulate = false;
            }
            else
            {
                target_array[i] = first_value && second_value;
            }
        }
        else if (first_value && second_value)
        {
            target_array[i] = false;
            accumulate = true;
        }
        else
        {
            target_array[i] = first_value || second_value;
        }
    }
}
