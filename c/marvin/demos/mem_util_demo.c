#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS.
#include <stdbool.h>
#include <string.h> // for memcpy, memcmp.
#include <assert.h>

#include "../memory_utilities/memory_blocks_operations.h"

static void assert_swapping(const void* const first_ptr,
    const void* const second_ptr, const size_t block_size)
{
    unsigned char a[block_size];
    memcpy(a, first_ptr, block_size);

    unsigned char b[block_size];
    memcpy(b, second_ptr, block_size);

    mrvn_swap_voids(&a, &b, block_size);

    assert(blocks_equal(a, second_ptr, block_size)
           && blocks_equal(b, first_ptr, block_size));
}

static void test_swapping(void)
{
    double a = 3.5;
    double b = 4.2;

    assert_swapping(&a, &b, sizeof(a));

    char c1[] = "Hello";
    char c2[] = "World";
    assert_swapping(c1, c2, sizeof(c1));

    printf("Tests succeeded\n");
}

int main(void)
{
    test_swapping();

    return EXIT_SUCCESS;
}
