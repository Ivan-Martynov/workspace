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

    struct linked_list_d* head_ptr = make_linked_list_from_array(n, array);
    linked_list_d_push_back(&head_ptr, -7.0);

    double second_array[] = {-2.0, 8.0, -4.0};
    struct linked_list_d* second_list_ptr
        = make_linked_list_from_array(3, second_array);

    linked_list_d_print(second_list_ptr);
    printf("\n");

    linked_list_d_append(&head_ptr, second_list_ptr);

    linked_list_d_free(&second_list_ptr);

    linked_list_d_print(head_ptr);
    printf("\n");

    map_linked_list(head_ptr, square);

    linked_list_reverse(head_ptr);

    linked_list_d_print(head_ptr);
    printf("\n");

    linked_list_d_free(&head_ptr);
}

void test_list_of_lists()
{
    double array_1[] = {11.5, 22.3};
    struct list_of_lists_d* lol_11_ptr
        = make_list_of_lists(make_list_node_d_from_array(2, array_1));

    double array_2[] = {328.0, 14.7};
    list_of_lists_d_push_back(
        &lol_11_ptr, make_list_node_d_from_array(2, array_2));

    double array_3[] = {5.0, -16.8, -7.0};
    struct list_of_lists_d* lol_21_ptr
        = make_list_of_lists(make_list_node_d_from_array(3, array_3));

    double array_4[] = {8.0, 9.0, -0.0, -1.2, 0.0, 3.0};
    list_of_lists_d_push_back(
        &lol_21_ptr, make_list_node_d_from_array(6, array_4));

    struct list_of_lists_d* lol_1_ptr
        = make_list_of_lists(make_list_node_d(NAN, lol_11_ptr));
    list_of_lists_d_push_back(&lol_1_ptr, make_list_node_d(NAN, lol_21_ptr));

    struct list_of_lists_d* lol_ptr = NULL;
    list_of_lists_d_push_front(&lol_ptr, make_list_node_d(NAN, lol_1_ptr));

    // list_of_lists_d_map(lol_ptr, square);

    struct list_of_lists_d* empty_ptr
        = make_list_of_lists(make_list_node_d(NAN, NULL));
    printf("Empty => ");
    print_list_of_lists(empty_ptr);
    printf("\n");
    printf("lol_11_ptr => ");
    print_list_of_lists(lol_11_ptr);
    printf("\n");
    printf("lol_1_ptr => ");
    print_list_of_lists(lol_1_ptr);
    printf("\n");
    printf("lol_ptr => ");
    print_list_of_lists(lol_ptr);
    printf("\n");

    struct list_of_lists_d* target_ptr = NULL;
    deep_reverse_list_of_lists(
        &target_ptr,
        lol_ptr);
    printf("lol_ptr deep reversed => ");
    print_list_of_lists(target_ptr);
    printf("\n");
    list_of_lists_free(&target_ptr);

    struct linked_list_d* list_ptr = NULL;
    fringe_list_of_lists(&list_ptr, lol_ptr);

    printf("lol_ptr fringed => ");
    linked_list_d_print(list_ptr);
    printf("\n");

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

void test_sets_of_set()
{
    //double array[] = {9.0, 5.0};
    //double array[] = {1.0, 9.0, 5.0};
    double array[] = {[0] = 1.0, [1] = 2.0, [2] = 3.0, [3] = 4.0};
    struct list_node_d* node_ptr
        = make_list_node_d_from_array(sizeof(array) / sizeof(*array), array);

    struct list_of_lists_d* target_ptr = NULL;
    make_sets_from_list_node_d(&target_ptr, node_ptr);

    print_list_of_lists(target_ptr);
    printf("\n");
    list_of_lists_free(&target_ptr);
}

int main()
{
    //test_vec_reverse();
    //test_map();
    //test_linked_list();
    //test_list_of_lists();
    //test_nan();
    test_sets_of_set();

    return 0;
}
