#include "size_info.h"

#include <stdio.h>

void test_char(const unsigned char c)
{
    printf("%d = %c\n", c, c);
}

int main()
{
#if 0
    printf("\n--- Limits for integer types ---\n");
    show_integer_limits();

    printf("\n--- Limits for floating point types ---\n");
    show_float_limits();
#endif

    const unsigned char x = 100;
    const unsigned char y = 200;
    test_char(x);
    test_char(y);
    test_char(x + y);

    const int z = (int)x + (int)y - 256;
    if (z == (char)(x + y))
    {
        printf("Success!\n");
    }
    else{
        printf("z = %d", z);
    }

    return 0;
}