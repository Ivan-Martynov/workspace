#include <stdio.h>

#include "functions.h"
#include "data_structures/list_operations.h"
#include "data_structures/linked_list_d.h"
#include "data_structures/list_of_lists_d.h"

#include <string.h>
#include <inttypes.h>
#include <math.h>

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

    linked_list_d_free(&second_list);

    linked_list_d_print(head);
    printf("\n");

    map_linked_list(head, square);

    linked_list_reverse(head);

    linked_list_d_print(head);
    printf("\n");

    linked_list_d_free(&head);
}

void test_list_of_lists()
{
    double array_1[] = {11.5, 22.3};
    struct list_of_lists_d* lol_11_ptr
        = make_list_of_lists(make_list_node_d_from_array(2, array_1));

    double array_2[] = {328.0, 14.7};
    lol_11_ptr->next_ptr
        = make_list_of_lists(make_list_node_d_from_array(2, array_2));

    double array_3[] = {5.0, -16.8, -7.0};
    struct list_of_lists_d* lol_21_ptr
        = make_list_of_lists(make_list_node_d_from_array(3, array_3));

    double array_4[] = {8.0, 9.0, -0.0, -1.2, 0.0, 3.0};
    lol_21_ptr->next_ptr
        = make_list_of_lists(make_list_node_d_from_array(6, array_4));

    struct list_of_lists_d* lol_1_ptr
        = make_list_of_lists(make_list_node_d(NAN, lol_11_ptr));

    struct list_of_lists_d* lol_2_ptr
        = make_list_of_lists(make_list_node_d(NAN, lol_21_ptr));
    lol_1_ptr->next_ptr = lol_2_ptr;

    struct list_of_lists_d* lol_ptr
        = make_list_of_lists(make_list_node_d(NAN, lol_1_ptr));

    //list_of_lists_d_map(lol_ptr, square);

    print_list_of_lists(lol_ptr);
    printf("\n");

#if 1 // TODO: Need to fix this.
    struct list_of_lists_d* target_ptr = reverse_list_of_lists(lol_ptr);
    //deep_reverse_list_of_lists(
    //    &target_ptr,
    //    lol_ptr);
    //print_list_of_lists(target_ptr);
    //printf("\n");
    //list_of_lists_free(&target_ptr);
#endif

    struct linked_list_d* list_ptr = NULL;
    fringe_list_of_lists(&list_ptr, lol_ptr);

    //linked_list_d_print(list_ptr);
    //printf("\n");

    linked_list_d_free(&list_ptr);
    list_of_lists_free(&lol_ptr);
}

void test_nan()
{
    const double d = NAN;
    uint64_t d_n;
    memcpy(&d_n, &d, sizeof(d));
    printf("Is %f nan? %d => %" PRIx64 "\n", d, isnan(d), d_n);
}

int main()
{
    //test_vec_reverse();
    //test_map();
    //test_linked_list();
    test_list_of_lists();

    //test_nan();

    return 0;
}
