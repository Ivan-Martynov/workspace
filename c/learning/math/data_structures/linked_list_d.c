#include "linked_list_d.h"
#include <stdlib.h>

struct linked_list_d* make_linked_list_d_node(const double value)
{
    struct linked_list_d* node_ptr = malloc(sizeof(*node_ptr));
    node_ptr->data = value;
    node_ptr->next_ptr = NULL;

    return node_ptr;
}

void linked_list_d_push_back(
    struct linked_list_d** const pp_head, const double value)
{
    if (*pp_head == NULL)
    {
        *pp_head = make_linked_list_d_node(value);
    }
    else
    {
        struct linked_list_d* last_ptr = *pp_head;

        while (last_ptr->next_ptr)
        {
            last_ptr = last_ptr->next_ptr;
        }

        last_ptr->next_ptr = make_linked_list_d_node(value);
    }
}

void linked_list_d_push_front(
    struct linked_list_d** const pp_head, const double value)
{
    struct linked_list_d* node_ptr = make_linked_list_d_node(value);
    node_ptr->next_ptr = *pp_head;

    *pp_head = node_ptr;
}

struct linked_list_d* linked_list_d_at(
    struct linked_list_d* const head_ptr, const size_t index)
{
    struct linked_list_d* current_ptr = head_ptr;

    for (size_t i = 0; i < index; ++i)
    {
        current_ptr = current_ptr->next_ptr;
    }

    return current_ptr;
}

struct linked_list_d* linked_list_d_last(struct linked_list_d* head_ptr)
{
    if (!head_ptr)
    {
        return NULL;
    }

    while (head_ptr->next_ptr)
    {
        head_ptr = head_ptr->next_ptr;
    }

    return head_ptr;
}

void linked_list_d_append(struct linked_list_d** const pp_target,
    const struct linked_list_d* src_ptr)
{
    while (src_ptr)
    {
        linked_list_d_push_back(pp_target, src_ptr->data);
        src_ptr = src_ptr->next_ptr;
    }
}

void linked_list_d_free(struct linked_list_d** const pp_head)
{
    struct linked_list_d* current_ptr = *pp_head;

    while (current_ptr)
    {
        struct linked_list_d* next_ptr = current_ptr->next_ptr;

        free(current_ptr);

        current_ptr = next_ptr;
    }

    *pp_head = NULL;
}

struct linked_list_d* make_linked_list_from_array(
    const size_t size, const double* const array)
{
    struct linked_list_d* head_ptr = NULL;

    for (size_t i = size - 1; i < size; --i)
    {
        linked_list_d_push_front(&head_ptr, array[i]);
    }

    return head_ptr;
}

size_t linked_list_d_size(const struct linked_list_d* head_ptr)
{
    size_t n = 0;

    for (; head_ptr; head_ptr = head_ptr->next_ptr)
    {
        ++n;
    }

    return n;
}
