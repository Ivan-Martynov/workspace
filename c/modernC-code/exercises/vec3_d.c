#include "vec3_d.h"

struct vec3_d
{
    double x;
    double y;
    double z;
};

struct vec3_d* vec3_d_init(struct vec3_d* const target_ptr, const double x,
    const double y, const double z)
{
    if (target_ptr)
    {
        target_ptr->x = x;
        target_ptr->y = y;
        target_ptr->z = z;
    }

    return target_ptr;
}

struct vec3_d* vec3_d_new(const double x, const double y, const double z)
{
    return vec3_d_init(malloc(sizeof(struct vec3_d)), x, y, z);
}

void vec3_d_delete(struct vec3_d* vec_ptr)
{
    if (vec_ptr)
    {
        free(vec_ptr);
        *vec_ptr = (struct vec3_d) {0};
    }
}
