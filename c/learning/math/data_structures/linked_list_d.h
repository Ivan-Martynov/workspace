#ifndef _LINKED_LIST_D_H_
#define _LINKED_LIST_D_H_

struct linked_list_d
{
    double data;
    struct linked_list_d* next;
};


struct linked_list_d* make_linked_list_d_node(const double);

struct linked_list_d* make_linked_list_from_array(const size_t, const double* const);

#endif // _LINKED_LIST_D_H_