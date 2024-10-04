#include "buffer.h"
#include "defs.h"

void insert_at(size_t arr[BUFFER_SIZE], const size_t index, const size_t value)
{
    if (index < BUFFER_SIZE)
    {
        arr[index] = value;
    }
}
