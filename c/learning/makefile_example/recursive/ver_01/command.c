#include "command.h"
#include "defs.h"

void run_command(void cmd(const void* const), const void* const ptr)
{
    if (ptr != NULL_POINTER)
    {
        cmd(ptr);
    }
}
