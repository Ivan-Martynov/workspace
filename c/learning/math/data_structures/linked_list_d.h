#ifndef _LINKED_LIST_D_H_
#define _LINKED_LIST_D_H_

#include <stddef.h>

struct linked_list_d
{
    double data;
    struct linked_list_d* next;
};


struct linked_list_d* make_linked_list_d_node(const double);

struct linked_list_d* make_linked_list_from_array(const size_t, const double* const);

void linked_list_d_push_back(struct linked_list_d**, const double);

struct linked_list_d* linked_list_d_at(
    struct linked_list_d* const, const size_t);

struct linked_list_d* linked_list_d_last(struct linked_list_d* const);

void linked_list_d_append(struct linked_list_d**, struct linked_list_d* const);

void linked_list_d_free(struct linked_list_d** const);

void linked_list_d_print(const struct linked_list_d*);

size_t linked_list_d_size(const struct linked_list_d*);

void linked_list_reverse(struct linked_list_d*);

#endif // _LINKED_LIST_D_H_