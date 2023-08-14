#include "binary_tree_d.h"

struct binary_tree_d* make_binary_tree_d_node(const double value)
{
    struct binary_tree_d* node = malloc(sizeof(*node));
    node->data = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}
