#include <stdio.h>
#include <stdlib.h>

#if 1
void fgoto(const unsigned int n)
{
    unsigned int j = 0;

AGAIN:
    printf("%u: p and q are %s, *p is %u\n", j + 1, j ? "equal" : "unequal", j);
    if (++j < n)
    {
        goto AGAIN;
    }
}

#else

void fgoto(const unsigned int n)
{
    unsigned int j = 0;
    unsigned int* p = 0;
    unsigned int* q;

AGAIN:
    if (p)
    {
        printf("%u: p and q are %s, *p is %u\n", j,
            (q == p) ? "equal" : "unequal", *p);
    }

    q = p;
    p = &((unsigned int) {
        j,
    });

    if (++j <= n)
    {
        goto AGAIN;
    }
}
#endif

int main(void)
{
    fgoto(4);

    return EXIT_SUCCESS;
}