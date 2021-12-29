#include <stdio.h>
#include <stdlib.h>

/* Function print_binary:
 * ------------------------
 *  displays x in binary
 *  format (zeros and ones)
 */
void print_binary(unsigned int x)
{
    int i;

    for (i = (sizeof(int) * 8) - 1; i >= 0; i--)
        (x & (1 << i)) ? putchar('1') : putchar('0');
#if 0
    while (x) {
        if (x & 1)
            printf("1");
        else
            printf("0");
        x >>= 1;
    }
#endif
    printf("\n");
}

/* Function bitwise_add:
 * -------------------------
 *  implementation of addition
 */
int bitwise_add(unsigned int x, unsigned int y)
{
    unsigned int r;
    while (x) {
        r = (y & x) << 1;
        y ^= x;
        x = r;
    }

    return y;
}

/* Function bitwise_multiply:
 * -------------------------
 *  implementation of muliplication
 */
int bitwise_multiply(unsigned int x, unsigned int y)
{
    if (x < y)
        return bitwise_multiply(y, x);

    unsigned int r = 0;
    while (y) {
        if (y & 1)
            r = bitwise_add(r, x);
        x <<= 1;
        y >>= 1;
    }

    return r;
}


/* Function xorSwap:
 * -----------------------------------
 *  swaps two variables.
 *  Note: the use is discouraged, it is
 *  here only for demonstration purpose
 */
void xorSwap(int *x, int *y)
{
    if (x != y) {
        *x ^= *y;
        *y ^= *x;
        *x ^= *y;
    }
    /* shorter version, still not encouraged
    *x = *x ^ *y ^ (*y = *x);
    */
}

/* Function is_odd:
 * -------------------------------
 *  compares the last bit with one
 */
int is_odd(int x)
{
    return (x & 1);
}

int main(int argc, char **argv)
{
    int x = argc < 2 ? 7 : atoi(argv[1]);
    /* if (x < 0) x = -x; */
    int y = 12;

    printf("Binary represenation of %d: ", x);
    print_binary(x);

    printf("Bitwise addition: %d\n", bitwise_add(x, y));
    printf("Bitwise multiplication: %d\n", bitwise_multiply(x, y));
    printf("Bitwise negation: NOT %d = %d\n", x, ~x);

    if (is_odd(x))
        printf("Bitwise parity: (%d & 1) => odd\n", x);
    else
        printf("Bitwise parity: (%d & 1) => even\n", x);

    xorSwap(&x, &y);
    printf("Bitwise swap: %d and %d\n", x, y);

    return 0;
}
