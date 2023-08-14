#ifndef _BINARY_TREE_D_H_
#define _BINARY_TREE_D_H_

#include <stddef.h>

struct binary_tree_d
{
    double data;
    struct binary_tree_d* left;
    struct binary_tree_d* right;
};

struct binary_tree_d make_node(const double);

#endif // _BINARY_TREE_D_H_
