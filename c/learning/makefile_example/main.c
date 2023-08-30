#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "defs.h"

int main(void)
{
    void* empty_ptr = NULL_POINTER;
    assert(empty_ptr);

    return EXIT_SUCCESS;
}