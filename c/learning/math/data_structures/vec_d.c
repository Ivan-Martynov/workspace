#include "vec_d.h"

#include <stdlib.h>

static const size_t default_capacity = 4;

static void* vec_d_realloc(struct vec_d* const vec_ptr, const size_t cap)
{
    vec_ptr->data = realloc(vec_ptr->data, cap * (sizeof(double)));

    if (vec_ptr->data)
    {
        vec_ptr->capacity = cap;
    }

    return vec_ptr->data;
}

static struct vec_d init_vec_d_empty()
{
    return (struct vec_d) {.data = NULL, .size = 0, .capacity = 0};
}

struct vec_d make_vec_d_empty()
{
    struct vec_d vec = init_vec_d_empty();

    vec_d_realloc(&vec, default_capacity);

    return vec;
}

void vec_d_push_back(struct vec_d* const vec_ptr, const double value)
{
    if ((vec_ptr->size >= vec_ptr->capacity)
        && (!vec_d_realloc(vec_ptr, vec_ptr->size + default_capacity)))
    {
        return;
    }

    vec_ptr->data[vec_ptr->size++] = value;
}

struct vec_d make_vec_d_from_array(
    const size_t count, const double arr[static 1])
{
    struct vec_d vec = init_vec_d_empty();

    if (vec_d_realloc(&vec, count))
    {
        vec.size = count;

        for (size_t i = 0; i < vec.size; ++i)
        {
            vec.data[i] = arr[i];
        }
    }

    return vec;
}

double vec_d_at(const struct vec_d* const vec_ptr, const size_t i)
{
    return vec_ptr->data[i];
}

void vec_d_free(struct vec_d* const vec_ptr)
{
    free(vec_ptr->data);
    vec_ptr->data = NULL;
}

void vec_d_append(
    struct vec_d* const target_ptr, const struct vec_d* const src_ptr)
{
    const size_t count = target_ptr->size + src_ptr->size;
    if ((target_ptr->capacity < count) && !vec_d_realloc(target_ptr, count))
    {
        return;
    }

    for (size_t i = 0; i < src_ptr->size; ++i)
    {
        target_ptr->data[target_ptr->size + i] = vec_d_at(src_ptr, i);
    }
    target_ptr->size = count;
}
