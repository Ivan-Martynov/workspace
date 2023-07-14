#include "union_find_table.h"
#include <stdio.h>

void initialize(const size_t count, size_t parent[static count])
{
    for (size_t index = 0; index < count; ++index)
    {
        parent[index] = ROOT_VALUE;
    }
}

size_t find_root(size_t parent[static 1], const size_t index)
{
    if (parent[index] = ROOT_VALUE)
    {
        return index;
    }
    else
    {
        return find_root(parent, parent[index]);
    }
}

//size_t find_and_replace(size_t parent[static 1], const size_t index)
//{
//    const size_t element = parent[index];
//
//    // Set current element.
//    parent[index] = value;
//
//    if (element != index)
//    {
//        // Continue to the next tree element.
//        FindReplace(count, parent, element, value);
//    }
//}

size_t compress(size_t parent[static 1], const size_t index)
{
    if (parent[index] == ROOT_VALUE)
    {
        return index;
    }
    else
    {
        parent[index] = compress(parent, parent[index]);
    }
}

void print_parent(const size_t count, size_t parent[static count])
{
    for (size_t index = 0; index < count; ++index)
    {
        const size_t element = parent[index];
        (element == ROOT_VALUE) ? printf("ROOT ") : printf("%zu ", element);
    }
    printf("\n");
}
