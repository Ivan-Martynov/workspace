#include "ProjectConfig.h"
#include "defs.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

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

    printf("Main directory: %s\n", MAIN_DIR);

    return EXIT_SUCCESS;
}
