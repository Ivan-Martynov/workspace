#include "tree_operations.h"

#include <stdio.h>

void print_binary_tree_d(const struct binary_tree_d* const tree_ptr)
{
    if (!tree_ptr)
    {
        return;
    }

    if (binary_tree_d_is_leaf(tree_ptr))
    {
        printf("%f ", tree_ptr->data);
    }
    else
    {
        print_binary_tree_d(tree_ptr->left);
        print_binary_tree_d(tree_ptr->right);
    }
}

size_t count_leaves_binary_tree_d(const struct binary_tree_d* const tree_ptr)
{
    if (!tree_ptr)
    {
        return 0;
    }

    if (binary_tree_d_is_leaf(tree_ptr))
    {
        return 1;
    }
    else
    {
        return count_leaves_binary_tree_d(tree_ptr->left)
               + count_leaves_binary_tree_d(tree_ptr->right);
    }
}
