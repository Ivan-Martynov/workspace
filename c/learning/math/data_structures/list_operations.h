#ifndef _LIST_OPERATIONS_H_
#define _LIST_OPERATIONS_H_

#include "vec_d.h"
#include "linked_list_d.h"
#include "list_of_lists_d.h"

double vec_d_get_last(const struct vec_d* const);

void vec_d_reverse(struct vec_d* const);

void for_each_list(double*, const size_t, void (*)(const double));

void map_list(double*, const size_t, double (*)(const double));

void map_linked_list(struct linked_list_d*, double (*)(const double));

struct linked_list_d* reverse_of_linked_list(struct linked_list_d*);
void linked_list_reverse(struct linked_list_d*);

void linked_list_d_print(const struct linked_list_d*);
void print_list_of_lists(struct list_of_lists_d*);

void fringe_list_of_lists(struct linked_list_d**, struct list_of_lists_d*);

void list_of_lists_d_map(
    struct list_of_lists_d*, double (*)(const double));

void deep_reverse_list_of_lists(
    struct list_of_lists_d**, struct list_of_lists_d*);

void make_sets_from_list_node_d(struct list_of_lists_d**, struct list_node_d*);

double accumulate_list_d(
    struct linked_list_d*, double (*)(double, double), double);

#endif // _LIST_OPERATIONS_H_
