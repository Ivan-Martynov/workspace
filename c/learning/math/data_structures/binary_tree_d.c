#include "binary_tree_d.h"

#include <stdlib.h>

struct binary_tree_d* make_binary_tree_d_node(const double value)
{
    struct binary_tree_d* node_ptr = malloc(sizeof(*node_ptr));
    node_ptr->data = value;
    node_ptr->left = NULL;
    node_ptr->right = NULL;

    return node_ptr;
}

bool binary_tree_d_is_leaf(const struct binary_tree_d* const node_ptr)
{
    return !(node_ptr->left) && !(node_ptr->right);
}

void binary_tree_d_free(struct binary_tree_d** const pp_tree)
{
    struct binary_tree_d* node_ptr = *pp_tree;

    if (node_ptr)
    {
        struct binary_tree_d* left_ptr = node_ptr->left;
        struct binary_tree_d* right_ptr = node_ptr->right;

        free(node_ptr);

        binary_tree_d_free(&left_ptr);
        binary_tree_d_free(&right_ptr);
    }

    *pp_tree = NULL;
}
