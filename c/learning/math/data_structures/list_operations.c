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
    double* array, const size_t count, const void func(const double))
{
    for (size_t i = 0; i < count; ++i)
    {
        func(array[i]);
    }
}

void map_list(
    double* array, const size_t count, const double func(const double))
{
    for (size_t i = 0; i < count; ++i)
    {
        array[i] = func(array[i]);
    }
}

void map_linked_list(
    struct linked_list_d* head, const double func(const double))
{
    while (head)
    {
        head->data = func(head->data);
        head = head->next_ptr;
    }
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
        printf("%f", head_ptr->data);
        head_ptr = head_ptr->next_ptr;
    }

    while (head_ptr)
    {
        printf(", %f", head_ptr->data);
        head_ptr = head_ptr->next_ptr;
    }
}

static void list_of_lists_d_map_helper(
    struct list_node_d* node_ptr, const double func(const double))
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
    struct list_of_lists_d* lists_ptr, const double func(const double))
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
        printf("%f", node_ptr->data);
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

    if (src_ptr->list_ptr)
    {
        // printf("Reverse nodes step 1\n");
        // printf("Target is null? %d\n", (pp_target == NULL));
         deep_reverse_list_of_lists(
            &((*pp_target)->list_ptr), src_ptr->list_ptr);
        //deep_reverse_list_of_lists(pp_target, src_ptr->list_ptr);
    }
    else
    {
        //printf("Reverse nodes step 2\n");
        deep_reverse_list_node_d(pp_target, src_ptr->next_ptr);
        list_node_d_push_back(pp_target, src_ptr->data, NULL);
    }
}

struct list_of_lists_d* reverse_list_of_lists(struct list_of_lists_d* lists_ptr)
{
    if (!lists_ptr)
    {
        return NULL;
    }
    else
    {
        //return reverse_list_of_lists(lists_ptr->next_ptr);
        struct list_node_d* node_ptr = NULL;
        // printf("Reverse lists step 1\n");
        // deep_reverse_list_of_lists(pp_target, lists_ptr->next_ptr);
        // printf("Reverse lists step 2\n");
        deep_reverse_list_node_d(&node_ptr, lists_ptr->node_ptr);

        if (!node_ptr)
        {
            // node_ptr = make_list_node_d(NAN, lists_ptr);
            printf("Empty list.\n");
            return make_list_of_lists(make_list_node_d(
                NAN, reverse_list_of_lists(lists_ptr->next_ptr)));
        }
        else
        {
            print_list_node_d(node_ptr);
            printf("\n");
            return make_list_of_lists(make_list_node_d(NAN, node_ptr->list_ptr));
            // list_of_lists_d_push_back(pp_target, node_ptr);
        }
    }
}

void deep_reverse_list_of_lists(
    struct list_of_lists_d** pp_target,
    struct list_of_lists_d* lists_ptr)
{
    if (!lists_ptr)
    {
        return;
    }
    else
    {
        deep_reverse_list_of_lists(pp_target, lists_ptr->next_ptr);

        struct list_node_d* node_ptr = NULL;
        // printf("Reverse lists step 1\n");
        // deep_reverse_list_of_lists(pp_target, lists_ptr->next_ptr);
        // printf("Reverse lists step 2\n");
        deep_reverse_list_node_d(&node_ptr, lists_ptr->node_ptr);

        if (!node_ptr)
        {
            //node_ptr = make_list_node_d(NAN, lists_ptr);
            printf("Empty list.\n");
            *pp_target = make_list_of_lists(make_list_node_d(NAN, *pp_target));
        }
        else
        {
            print_list_node_d(node_ptr);
            printf("\n");
            //list_of_lists_d_push_back(pp_target, node_ptr);
        }

        // list_node_d_free(&node_ptr);
        // printf("Reverse lists step 3\n");
        // list_of_lists_d_push_back(pp_target, node_ptr);
        // lists_ptr = lists_ptr->next_ptr;
    }
}
