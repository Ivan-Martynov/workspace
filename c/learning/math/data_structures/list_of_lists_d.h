#ifndef _LIST_OF_LISTS_D_H
#define _LIST_OF_LISTS_D_H

#include <stddef.h>

struct list_node_d
{
    double data;
    struct list_node_d* next_ptr;
    struct list_of_lists_d* list_ptr;
};

struct list_of_lists_d
{
    struct list_node_d* node_ptr;
    struct list_of_lists_d* next_ptr;
};

struct list_node_d* make_list_node_d(const double, struct list_of_lists_d*);

void list_node_d_push_front(
    struct list_node_d** const, const double, struct list_of_lists_d*);
void list_node_d_push_back(
    struct list_node_d** const, const double, struct list_of_lists_d*);

void list_of_lists_d_push_front(
    struct list_of_lists_d** const, struct list_node_d*);
void list_of_lists_d_push_back(
    struct list_of_lists_d** const, struct list_node_d*);

struct list_node_d* make_list_node_d_from_array(
    const size_t, const double* const);

struct list_of_lists_d* make_list_of_lists(struct list_node_d*);

void list_node_d_free(struct list_node_d** const);
void list_of_lists_free(struct list_of_lists_d** const);


#endif // _LIST_OF_LISTS_D_H