#include "buffer.h"
#include "defs.h"

size_t search_for(const size_t arr[BUFFER_SIZE], const size_t value)
{
    for (size_t i = 0; i < BUFFER_SIZE; ++i)
    {
        if (arr[i] == value)
        {
            return i;
        }
    }

    return -1;
}
