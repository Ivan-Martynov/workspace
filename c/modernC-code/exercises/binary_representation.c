#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

union double_ptr_inspect {
    const double* const val_ptr;
    unsigned char bytes[sizeof(double*)];
};

union unsigned_inspect {
    unsigned val;
    unsigned char bytes[sizeof(unsigned)];
};

void print_uchar_nth_bit(const unsigned char c, const size_t bit_index)
{
    printf((c & (1ULL << bit_index)) ? "1" : "0");
}

void print_uchar_as_bin_little_end(const unsigned char c)
{
    for (size_t i = CHAR_BIT - 1; i < CHAR_BIT; --i)
    {
        print_uchar_nth_bit(c, i);
    }
}

void print_uchar_as_bin_big_end(const unsigned char c)
{
    for (size_t i = 0; i < CHAR_BIT; ++i)
    {
        print_uchar_nth_bit(c, i);
    }
}

void print_uchar_array_as_bin_little_end(
    const size_t n, const unsigned char arr[static n])
{
    print_uchar_as_bin_little_end(arr[n - 1]);
    for (size_t i = n - 2; i < n; --i)
    {
        printf(" ");
        print_uchar_as_bin_little_end(arr[i]);
    }
    printf("\n");
}

void print_uchar_array_as_bin_big_end(
    const size_t n, const unsigned char arr[static n])
{
    print_uchar_as_bin_big_end(arr[0]);
    for (size_t i = 1; i < n; ++i)
    {
        printf(" ");
        print_uchar_as_bin_big_end(arr[i]);
    }
    printf("\n");
}

void test_double_ptr(const union double_ptr_inspect two_fold)
{
    printf("Value is %g => %p\n", *two_fold.val_ptr, (void*)two_fold.val_ptr);
    for (size_t i = 0; i < sizeof(two_fold.bytes); ++i)
    {
        printf("byte[%zu]: 0x%.02hhX\n", i, two_fold.bytes[i]);
    }
    print_uchar_array_as_bin_little_end(sizeof(two_fold.bytes), two_fold.bytes);
}

void test_unsigned(const union unsigned_inspect two_fold)
{
    printf("Value is 0x%.08X => %u\n", two_fold.val, two_fold.val);
    for (size_t i = 0; i < sizeof(two_fold.bytes); ++i)
    {
        printf("byte[%zu]: 0x%.02hhX => %u\n", i, two_fold.bytes[i],
            two_fold.bytes[i]);
    }
    print_uchar_array_as_bin_little_end(sizeof(two_fold.bytes), two_fold.bytes);
}

int main(void)
{
    printf("Char bit = %u\n", CHAR_BIT);

    const union unsigned_inspect two_fold = {.val = 0xAABBCCDD};
    test_unsigned(two_fold);
    test_unsigned((union unsigned_inspect) {.val = 1832});

    const double a[] = {3.28, 5.789};
    test_double_ptr((union double_ptr_inspect) {.val_ptr = &a[0]});
    test_double_ptr((union double_ptr_inspect) {.val_ptr = &a[1]});

    double b = 827.3028;
    unsigned char* c_ptr = (unsigned char*)&b;
    printf("Value is %g\n", b);
    for (size_t i = 0; i < sizeof(b); ++i)
    {
        printf("byte[%zu]: 0x%.hhX => %u\n", i, c_ptr[i], c_ptr[i]);
    }
    print_uchar_array_as_bin_little_end(sizeof(b), c_ptr);

    return EXIT_SUCCESS;
}
