#include "list_operations.h"
#include <stdio.h>
#include <math.h>

double vec_d_get_last(const struct vec_d* const vec_ptr)
{
    return vec_d_at(vec_ptr, vec_ptr->size - 1);
}

static void reverse(size_t count, double* const arr)
{
    const size_t middle = count >> 1;

    --count; // point to last element.

    for (size_t i = 0; i < middle; ++i)
    {
        const size_t symmetric = count - i;

        double temp = arr[i];

        arr[i] = arr[symmetric];

        arr[symmetric] = temp;
    }
}

void vec_d_reverse(struct vec_d* const vec_ptr)
{
    reverse(vec_ptr->size, vec_ptr->data);
}

void for_each_list(
    double* array, const size_t count, void func(const double))
{
    for (size_t i = 0; i < count; ++i)
    {
        func(array[i]);
    }
}

void map_list(
    double* array, const size_t count, double func(const double))
{
    for (size_t i = 0; i < count; ++i)
    {
        array[i] = func(array[i]);
    }
}

void map_linked_list(
    struct linked_list_d* head, double func(const double))
{
    while (head)
    {
        head->data = func(head->data);
        head = head->next_ptr;
    }
}

struct linked_list_d* reverse_of_linked_list(struct linked_list_d* head_ptr)
{
    struct linked_list_d* result_ptr = NULL;

    while (head_ptr)
    {
        linked_list_d_push_front(&result_ptr, head_ptr->data);
        head_ptr = head_ptr->next_ptr;
    }

    return result_ptr;
}

void linked_list_reverse(struct linked_list_d* head_ptr)
{
    size_t size = linked_list_d_size(head_ptr);
    const size_t middle = size >> 1;

    --size;
    for (size_t i = 0; i < middle; ++i)
    {
        struct linked_list_d* first_ptr = linked_list_d_at(head_ptr, i);
        struct linked_list_d* second_ptr = linked_list_d_at(head_ptr, size - i);

        const double temp = first_ptr->data;
        first_ptr->data = second_ptr->data;
        second_ptr->data = temp;
    }
}

void linked_list_d_print(const struct linked_list_d* head_ptr)
{
    if (head_ptr)
    {
        printf("(%f", head_ptr->data);
        head_ptr = head_ptr->next_ptr;
    }

    while (head_ptr)
    {
        printf(", %f", head_ptr->data);
        head_ptr = head_ptr->next_ptr;
    }
    printf(")");
}

static void list_of_lists_d_map_helper(
    struct list_node_d* node_ptr, double func(const double))
{
    if (!node_ptr)
    {
        return;
    }

    if (node_ptr->list_ptr)
    {
        list_of_lists_d_map(node_ptr->list_ptr, func);
    }
    else
    {
        while (node_ptr)
        {
            node_ptr->data = func(node_ptr->data);
            node_ptr = node_ptr->next_ptr;
        }
    }
}

void list_of_lists_d_map(
    struct list_of_lists_d* lists_ptr, double func(const double))
{
    while (lists_ptr)
    {
        list_of_lists_d_map_helper(lists_ptr->node_ptr, func);
        lists_ptr = lists_ptr->next_ptr;
    }
}

void print_list_node_d(struct list_node_d* node_ptr)
{
    if (!node_ptr)
    {
        return;
    }

    if (node_ptr->list_ptr)
    {
        print_list_of_lists(node_ptr->list_ptr);
    }
    else
    {
        if (!isnan(node_ptr->data))
        {
            printf("%f", node_ptr->data);
        }
        node_ptr = node_ptr->next_ptr;

        while (node_ptr)
        {
            printf(", %f", node_ptr->data);
            node_ptr = node_ptr->next_ptr;
        }
    }
}

void print_list_of_lists(struct list_of_lists_d* lists_ptr)
{
    if (lists_ptr)
    {
        printf("(");
        print_list_node_d(lists_ptr->node_ptr);
        printf(")");

        lists_ptr = lists_ptr->next_ptr;
    }
    else
    {
        //printf("(");
        //printf(")");
    }

    while (lists_ptr)
    {
        printf(", (");
        print_list_node_d(lists_ptr->node_ptr);
        printf(")");

        lists_ptr = lists_ptr->next_ptr;
    }
}

static void fringe_list_of_lists_helper(
    struct linked_list_d** pp_target, struct list_node_d* node_ptr)
{
    if (!node_ptr)
    {
        return;
    }

    if (node_ptr->list_ptr)
    {
        fringe_list_of_lists(pp_target, node_ptr->list_ptr);
    }
    else
    {
        while (node_ptr)
        {
            linked_list_d_push_back(pp_target, node_ptr->data);
            node_ptr = node_ptr->next_ptr;
        }
    }
}

void fringe_list_of_lists(
    struct linked_list_d** pp_target, struct list_of_lists_d* lists_ptr)
{
    while (lists_ptr)
    {
        fringe_list_of_lists_helper(pp_target, lists_ptr->node_ptr);
        lists_ptr = lists_ptr->next_ptr;
    }
}

static void deep_reverse_list_node_d(
    struct list_node_d** pp_target, struct list_node_d* src_ptr)
{
    if (!src_ptr)
    {
        return;
    }

    if (isnan(src_ptr->data) && (src_ptr->list_ptr))
    {
        struct list_of_lists_d* inner_list_ptr = NULL;

        deep_reverse_list_of_lists(
            &inner_list_ptr, src_ptr->list_ptr);

        *pp_target = make_list_node_d(NAN, inner_list_ptr);
    }
    else
    {
        deep_reverse_list_node_d(pp_target, src_ptr->next_ptr);
        list_node_d_push_back(pp_target, src_ptr->data, NULL);
    }
}

void deep_reverse_list_of_lists(
    struct list_of_lists_d** pp_target, struct list_of_lists_d* lists_ptr)
{
    if (!lists_ptr)
    {
        return;
    }
    else
    {
        struct list_node_d* node_ptr = NULL;
        deep_reverse_list_node_d(&node_ptr, lists_ptr->node_ptr);

        list_of_lists_d_push_front(pp_target, node_ptr);
        deep_reverse_list_of_lists(pp_target, lists_ptr->next_ptr);
    }
}

static struct list_node_d* copy_list_node_d(struct list_node_d* node_ptr)
{
    struct list_node_d* copy_ptr = NULL;

    while (node_ptr)
    {
        list_node_d_push_back(&copy_ptr, node_ptr->data, node_ptr->list_ptr);
        node_ptr = node_ptr->next_ptr;
    }

    return copy_ptr;
}

void make_sets_from_list_node_d(
    struct list_of_lists_d** pp_target, struct list_node_d* node_ptr)
{
    if (!node_ptr)
    {
        return;
    }

    struct list_of_lists_d* temp_ptr = NULL;
    make_sets_from_list_node_d(&temp_ptr, node_ptr->next_ptr);

    struct list_node_d* copy_ptr = NULL;
    list_node_d_push_front(&copy_ptr, node_ptr->data, node_ptr->list_ptr);
    list_of_lists_d_push_back(pp_target, copy_ptr);

    while (temp_ptr)
    {
        copy_ptr = copy_list_node_d(temp_ptr->node_ptr);
        list_node_d_push_front(&copy_ptr, node_ptr->data, node_ptr->list_ptr);

        list_of_lists_d_push_back(pp_target, temp_ptr->node_ptr);
        list_of_lists_d_push_back(pp_target, copy_ptr);

        temp_ptr = temp_ptr->next_ptr;
    }
}

double accumulate_list_d(struct linked_list_d* node_ptr,
    double op(double, double), double initial)
{
    for (; node_ptr; node_ptr = node_ptr->next_ptr)
    {
        initial = op(initial, node_ptr->data);
    }

    return initial;
}
