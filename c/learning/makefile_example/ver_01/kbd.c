#include <stdio.h>

#include "command.h"
#include "defs.h"

static void kbd_check_pointer(const void* const ptr)
{
    if (ptr != NULL_POINTER)
    {
        printf("kbd: pointer's address: %p.\n", ptr);
    }
    else
    {
        printf("kbd: received null pointer.\n");
    }
}

void kbd_test(const void* const ptr)
{
    run_command(kbd_check_pointer, ptr);
}
