#include <stdio.h>

#include "buffer.h"
#include "command.h"

static void file_check_path(const void* const ptr)
{
    const char* path_ptr = (const char*)ptr;
    printf("files: checking path %s.\n", path_ptr);

    FILE* file_ptr = fopen(path_ptr, "r");
    if (file_ptr)
    {
        printf("File found, closing.\n");
        fclose(file_ptr);
    }
    else
    {
        printf("File not found.\n");
    }
}

void files_test(const void* const ptr)
{
    run_command(file_check_path, ptr);
}
