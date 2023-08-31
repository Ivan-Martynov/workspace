#include <stdlib.h>
#include "defs.h"

void util_pointer_check(void** ptr_ptr)
{
    if (*ptr_ptr)
    {
        free(*ptr_ptr);
        *ptr_ptr = NULL_POINTER;
    }
}
