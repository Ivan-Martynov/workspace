#include <stdio.h>

#include "functions.h"
#include "data_structures/list_operations.h"
#include "data_structures/linked_list_d.h"

static void debug_vec_d(const struct vec_d* const vec_ptr)
{
    if (vec_ptr->size > 0)
    {
        printf("%f", vec_d_at(vec_ptr, 0));

        for (size_t i = 1; i < vec_ptr->size; ++i)
        {
            printf(", %f", vec_d_at(vec_ptr, i));
        }

        printf("\n");
    }
}

void print_double(const double value)
{
    printf("%f ", value);
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
        head = head->next;
    }
}

void test_map()
{
    const size_t n = 5;
    double array[] = {1.0, 3.0, 2.0, 9.0, 5.0};
    struct vec_d vec = make_vec_d_from_array(n, array);

    double second_array[] = {4.0, 8.0, 12.0};
    struct vec_d second_vec = make_vec_d_from_array(3, second_array);

    vec_d_append(&vec, &second_vec);

    map_list(vec.data, vec.size, square);
    for_each_list(vec.data, vec.size, print_double);
    printf("\n");

    vec_d_free(&vec);
    vec_d_free(&second_vec);
}

void test_vec_reverse()
{
    const size_t n = 5;
    double array[] = {1.0, 3.0, 2.0, 9.0, 5.0};
    struct vec_d vec = make_vec_d_from_array(n, array);

    vec_d_push_back(&vec, 11.0);

    debug_vec_d(&vec);

    vec_d_reverse(&vec);

    debug_vec_d(&vec);

    vec_d_free(&vec);
}

void test_linked_list()
{
    const size_t n = 5;
    double array[] = {1.0, 3.0, 2.0, 9.0, 5.0};

    struct linked_list_d* head = make_linked_list_from_array(n, array);
    linked_list_d_push_back(&head, -7.0);

    double second_array[] = {-2.0, 8.0, -4.0};
    struct linked_list_d* second_list
        = make_linked_list_from_array(3, second_array);

    linked_list_d_append(&head, second_list);

    linked_list_d_print(head);

    map_linked_list(head, square);

    linked_list_reverse(head);

    linked_list_d_print(head);

    linked_list_d_free(&head);
}

int main()
{
    //test_vec_reverse();
    //test_map();

    test_linked_list();

    return 0;
}
