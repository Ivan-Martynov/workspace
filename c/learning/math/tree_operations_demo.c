#include "data_structures/binary_tree_d.h"
#include "data_structures/tree_operations.h"

#include <stdio.h>
#include <math.h>

void test_binary_tree()
{
    struct binary_tree_d* first_tree = make_binary_tree_d_node(NAN);
    first_tree->left = make_binary_tree_d_node(NAN);
    first_tree->left->left = make_binary_tree_d_node(NAN);
    first_tree->left->left->left = make_binary_tree_d_node(1.0);
    first_tree->left->left->right = make_binary_tree_d_node(2.0);

    first_tree->right = make_binary_tree_d_node(NAN);
    first_tree->right->left = make_binary_tree_d_node(3.0);
    first_tree->right->right = make_binary_tree_d_node(4.0);

    print_binary_tree_d(first_tree);
    printf("\n");

    printf(
        "First tree has %zu leaves.\n", count_leaves_binary_tree_d(first_tree));

    binary_tree_d_free(&first_tree);
}

int main()
{
    test_binary_tree();

    return 0;
}