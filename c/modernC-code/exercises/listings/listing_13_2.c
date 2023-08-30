#include <stdio.h>
#include <complex.h>
#include <tgmath.h>

unsigned i = 1;

int main(void)
{
    //double complex z1 = I * I;     // imaginary unit squared
    //printf("I * I = %.1f%+.1fi\n", creal(z1), cimag(z1));

    const double d = acos(0.3);

    unsigned i = (d != 0) ? 2 : 3;
    if (i)
    {
        extern unsigned i;
        printf("%u\n", i);
    }
    else
    {
        printf("%u\n", i);
    }
}
