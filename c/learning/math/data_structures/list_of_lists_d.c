#include "list_of_lists_d.h"
#include <stdlib.h>

struct list_node_d* make_list_node_d(
    const double value, struct list_of_lists_d* list_ptr)
{
    struct list_node_d* node_ptr = malloc(sizeof(*node_ptr));
    node_ptr->data = value;
    node_ptr->next_ptr = NULL;
    node_ptr->list_ptr = list_ptr;

    return node_ptr;
}

void list_node_d_push_back(struct list_node_d** const pp_head,
    const double value, struct list_of_lists_d* list_ptr)
{
    struct list_node_d* node_ptr = make_list_node_d(value, list_ptr);

    if (*pp_head)
    {
        struct list_node_d* last_ptr = *pp_head;

        while (last_ptr->next_ptr)
        {
            last_ptr = last_ptr->next_ptr;
        }

        last_ptr->next_ptr = node_ptr;
    }
    else
    {
        *pp_head = node_ptr;
    }
}

void list_of_lists_d_push_back(
    struct list_of_lists_d** const pp_head, struct list_node_d* node_ptr)
{
    struct list_of_lists_d* list_ptr = make_list_of_lists(node_ptr);

    if (*pp_head)
    {
        struct list_of_lists_d* last_ptr = *pp_head;

        while (last_ptr->next_ptr)
        {
            last_ptr = last_ptr->next_ptr;
        }

        last_ptr->next_ptr = list_ptr;
    }
    else
    {
        *pp_head = list_ptr;
    }
}

struct list_node_d* make_list_node_d_from_array(
    const size_t size, const double* const array)
{
    if (size == 0)
    {
        return NULL;
    }

    struct list_node_d* const head_ptr = make_list_node_d(array[0], NULL);

    struct list_node_d* current_ptr = head_ptr;

    for (size_t i = 1; i < size; ++i)
    {
        struct list_node_d* node_ptr = make_list_node_d(array[i], NULL);

        current_ptr->next_ptr = node_ptr;
        current_ptr = node_ptr;
    }

    return head_ptr;
}

struct list_of_lists_d* make_list_of_lists(struct list_node_d* node_ptr)
{
    struct list_of_lists_d* lists_ptr = malloc(sizeof(*lists_ptr));
    lists_ptr->node_ptr = node_ptr;
    lists_ptr->next_ptr = NULL;

    return lists_ptr;
}

void list_node_d_free(struct list_node_d** const pp_head)
{
    struct list_node_d* current_ptr = *pp_head;

    while (current_ptr)
    {
        struct list_node_d* next_ptr = current_ptr->next_ptr;

        list_of_lists_free(&(current_ptr->list_ptr));
        free(current_ptr);

        current_ptr = next_ptr;
    }

    *pp_head = NULL;
}

void list_of_lists_free(struct list_of_lists_d** const pp_list)
{
    struct list_of_lists_d* current_ptr = *pp_list;

    while (current_ptr)
    {
        struct list_of_lists_d* next_ptr = current_ptr->next_ptr;

        list_node_d_free(&(current_ptr->node_ptr));

        free(current_ptr);

        current_ptr = next_ptr;
    }

    *pp_list = NULL;
}

