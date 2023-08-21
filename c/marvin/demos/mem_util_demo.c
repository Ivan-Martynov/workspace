#include <stdio.h>
#include <stdlib.h>

#include "../memory_utilities/memory_blocks_operations.h"

static void test_swaps()
{
    double a = 3.5;
    double b = 4.2;

    printf("Values before swap: %g; %g\n", a, b);

    mrvn_swap_voids(&a, &b, sizeof(double));

    printf("Values after swap: %g; %g\n", a, b);
}

int main(void)
{
    test_swaps();

    return EXIT_SUCCESS;
}
