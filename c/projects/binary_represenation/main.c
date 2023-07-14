#include <stdio.h>

#include "binary_representation.h"

void array_binary_representation(long long num)
{
    bool a_bits[BIT_WIDTH];
    number_to_array(a_bits, num);

    char bits_string[BIT_WIDTH + CHUNK_COUNT - 1];
    array_to_string_representation(bits_string, a_bits);
#if 1
    printf("Array represenation for %lld = %s\n", num, bits_string);
#else

    printf("Array represenation for %lld = ", num);

    for (size_t i = 0; i < BIT_WIDTH; i += BITS_IN_BYTE)
    {
        for (size_t j = i; j < i + BITS_IN_BYTE; ++j)
        {
            printf("%d", a_bits[j]);
        }
        printf(" ");
    }
    printf("\n");
#endif

    printf("Test array back conversion: ");
    for (size_t i = 0; i < BIT_WIDTH; ++i)
    {
        printf("%d", a_bits[i]);
    }
    printf(" = %lld\n", array_to_number(a_bits));
}

long long dec_to_bin(long long num)
{
    unsigned long long int result = 0;
    unsigned long long int q = 1;
    while (num != 0)
    {
#if 0
        if (num & 1)
        {
            result += q;
        }
#else
        result += (num % 2) * q;
#endif

        q *= 10;
        num /= 2;
    }

    return result;
}

void test_sum(
    const long long int first_number, const long long int second_number)
{
    bool first_array[BIT_WIDTH];
    number_to_array(first_array, first_number);
    array_binary_representation(first_number);

    bool second_array[BIT_WIDTH];
    number_to_array(second_array, second_number);
    array_binary_representation(second_number);

    bool result_array[BIT_WIDTH];
    binary_sum(result_array, first_array, second_array);

    char bits_string[BIT_WIDTH + CHUNK_COUNT - 1];
    array_to_string_representation(bits_string, result_array);
    printf("String = %s\n", bits_string);
    printf("Sum %lld + %lld = %lld\n", first_number, second_number,
        array_to_number(result_array));
}

void show_decimal_as_binary(const long long int num)
{
    printf("Number %lld in binary = %lld\n", num, dec_to_bin(num));
    array_binary_representation(num);
}

int main(){
    printf("Enter first integer: ");
    long long int first_number;
    scanf("%lld", &first_number);

    show_decimal_as_binary(first_number);

    test_sum(2, 3);
    test_sum(123, 339);
    test_sum(-2, 3);
    test_sum(-3, 2);
    test_sum(-2, -3);
    test_sum(8, -8);
    test_sum(11, -28);
    test_sum(0, 0);

    //printf("%f\n", 0x1p-2);
    //printf("%f\n", 0x1p4);

    return 0;
}