#ifndef _BINARY_TREE_D_H_
#define _BINARY_TREE_D_H_

#include <stddef.h>
#include <stdbool.h>

struct binary_tree_d
{
    double data;
    struct binary_tree_d* left;
    struct binary_tree_d* right;
};

struct binary_tree_d* make_binary_tree_d_node(const double);

bool binary_tree_d_is_leaf(const struct binary_tree_d* const);

void binary_tree_d_free(struct binary_tree_d** const pp_tree);

#endif // _BINARY_TREE_D_H_
