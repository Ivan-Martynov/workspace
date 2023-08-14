#include "linked_list_d.h"

#include <stddef.h>

struct linked_list_d* make_linked_list_d_node(const double value)
{
    struct linked_list_d* node = malloc(sizeof(*node));
    node->data = value;
    node->next = NULL;

    return node;
}

struct linked_list_d* make_linked_list_from_array(
    const size_t size, const double* const array)
{
    if (size == 0)
    {
        return NULL;
    }

    struct linke_list_d* head = make_linked_list_d_node(array[0]);

    struct linke_list_d* current = head;

    for (size_t i = 1; i < size; ++i)
    {
        struct linke_list_d* node = make_linked_list_d_node(array[i]);
        current->next = node;

        current = node;
    }

    return head;
}
