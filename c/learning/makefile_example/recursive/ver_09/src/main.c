#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "defs.h"

int main(void)
{
    void* const empty_ptr = NULL_POINTER;
    assert(!empty_ptr);

    if (empty_ptr)
    {
        printf("Pointer's address = %p.\n", empty_ptr);
    }
    else
    {
        printf("Pointer is null.\n");
    }

    return EXIT_SUCCESS;
}
