#include <stdio.h>
#include <stdlib.h>

#if 0
void fgoto2(const unsigned int n)
{
    unsigned int j = 0;
    unsigned int* p = 0;
    unsigned int* q;

AGAIN:
    if (p)
    {
        printf("%u: p and q are %s, *p is %u\n", j,
            (j > 1) ? "equal" : "unequal", *p);
    }

    q = p;
    p = &j;

    if (++j <= n)
    {
        goto AGAIN;
    }
}
#endif

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

int main(void)
{
    fgoto(2);

    return EXIT_SUCCESS;
}