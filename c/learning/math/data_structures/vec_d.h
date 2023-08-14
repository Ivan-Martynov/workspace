#ifndef _VEC_D_H_
#define _VEC_D_H_

#include <stddef.h>

struct vec_d
{
    double* data;
    size_t size;
    size_t capacity;
};

struct vec_d make_vec_d_empty();
struct vec_d make_vec_d_from_array(const size_t, const double arr[static 1]);

void vec_d_push_back(struct vec_d* const, const double);

double vec_d_at(const struct vec_d* const, const size_t);

void vec_d_free(struct vec_d* const);

void vec_d_append(struct vec_d* const, const struct vec_d* const);

#endif //_VEC_D_H_
