#include <stdio.h>

#include "buffer.h"
#include "defs.h"

void display_array_info(const size_t arr[BUFFER_SIZE])
{
    printf("Default buffer size is %zu.\n", BUFFER_SIZE);
    if (arr != NULL_POINTER)
    {
        printf("Address of array's first element: %p.\n", (void*)arr);
    }
    else
    {
        printf("Array is invalid.\n");
    }
}
