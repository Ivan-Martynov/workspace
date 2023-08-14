#include "list_operations.h"

double vec_d_get_last(const struct vec_d* const vec_ptr)
{
    return vec_d_at(vec_ptr, vec_ptr->size - 1);
}

static void reverse(size_t count, double* const arr)
{
    const size_t middle = count >> 1;

    --count; // point to last element.

    for (size_t i = 0; i < middle; ++i)
    {
        const size_t symmetric = count - i;

        double temp = arr[i];

        arr[i] = arr[symmetric];

        arr[symmetric] = temp;
    }
}

void vec_d_reverse(struct vec_d* const vec_ptr)
{
    reverse(vec_ptr->size, vec_ptr->data);
}
