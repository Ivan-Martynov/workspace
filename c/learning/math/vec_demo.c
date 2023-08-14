#include <stdio.h>

#include "data_structures/vec_d.h"

static void debug_vec_d(const struct vec_d* const vec_ptr)
{
    if (vec_ptr->size > 0)
    {
        printf("%f", vec_d_at(vec_ptr, 0));

        for (size_t i = 1; i < vec_ptr->size; ++i)
        {
            printf(", %f", vec_d_at(vec_ptr, i));
        }

        printf("\n");
    }
}

int main()
{
    const size_t n = 5;
    const double array[] = {1.0, 3.0, 2.0, 9.0, 5.0};

    struct vec_d vec = make_vec_d_from_array(n, array);

    vec_d_push_back(&vec, 11.0);

    debug_vec_d(&vec);

    vec_d_free(&vec);

    return 0;
}