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
