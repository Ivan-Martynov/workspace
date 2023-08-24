#include <stdio.h>
#include <stdlib.h>

#include "circular.h"

int main(void)
{
    size_t len = 12;
    circular* c = circular_new(len);

    circular_append(c, 10.23);
    circular_append(c, 0.94);
    circular_append(c, -3.021);

    circular_fput(c, stdout);
    circular_delete(c);

    return EXIT_SUCCESS;
}