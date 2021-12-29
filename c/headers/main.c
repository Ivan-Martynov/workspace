#include <stdio.h>
#include "simplemath.h"

int main(int argc, char *argv[])
{
    int a = 10, b;

    b = plusone(a);

    printf("%d %d\n", a, b);

    return 0;
}
