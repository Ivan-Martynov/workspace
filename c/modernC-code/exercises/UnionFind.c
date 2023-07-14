#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "union_find_table.h"

void singleton_partition(const size_t count, size_t parent[static count])
{
    for (size_t index = 0; index < count; ++index)
    {
        parent[index] = SIZE_MAX;
    }
}

int FindRoot(size_t parent[static 1], const size_t count, const size_t index)
{
    if (parent[index] == SIZE_MAX)
    {
        return index;
    }

    return parent[index] == index ? index
                                  : FindRoot(parent, count, parent[index]);
}

// Initialize a singleton partition. Each element is the root of its own private
// set.
void make_set(const size_t count, size_t parent[static count])
{
    for (size_t index = 0; index < count; ++index)
    {
        parent[index] = index;
    }
}

// Find the root of a tree by index.
size_t Find(const size_t count, size_t parent[static count], const size_t index)
{
    if (index >= count)
    {
        return SIZE_MAX;
    }
    else
    {
        return (parent[index] == index) ? index
                                        : Find(count, parent, parent[index]);
    }
}

// Change/set all array entries to a give value on a path to the root
// inclusively.
void FindReplace(const size_t count, size_t parent[static count],
    const size_t index, const size_t value)
{
    if (index < count)
    {
        const size_t element = parent[index];

        // Set current element.
        parent[index] = value;

        if (element != index)
        {
            // Continue to the next tree element.
            FindReplace(count, parent, element, value);
        }
    }
}

// Change/set all array entries to the found root.
size_t FindCompress(
    const size_t count, size_t parent[static count], const size_t index)
{
    if (index >= count)
    {
        return SIZE_MAX;
    }
    else
    {
        if (parent[index] != index)
        {
            parent[index] = FindCompress(count, parent, parent[index]);
        }

        return parent[index];
    }
}

// For two given elements, combine their trees into one.
void Union(const size_t count, size_t parent[static count],
    const size_t first_index, const size_t second_index)
{
    if ((first_index < count) && (second_index < count))
    {
        const size_t value = FindCompress(count, parent, first_index);
        FindReplace(count, parent, second_index, value);
    }
}

void print_array(const size_t count, size_t parent[static count])
{
    printf("\n");
    for (size_t index = 0; index < count; ++index)
    {
        printf("%zu ", parent[index]);
    }
    printf("\n");
}

#define TABLE_SIZE 10

void test_table()
{
    const size_t count = 8;
    size_t parent[count];
    initialize(count, parent);
    print_parent(count, parent);

    parent[1] = 0;
    parent[2] = 1;
    parent[3] = 1;
    parent[4] = 2;
    parent[5] = 2;
    parent[6] = 4;
    parent[7] = 5;

    print_parent(count, parent);
    compress(parent, 6);
    print_parent(count, parent);
}

int main()
{
#if 0
    size_t parent[TABLE_SIZE];
    make_set(TABLE_SIZE, parent);
    assert(Find(TABLE_SIZE, parent, 0) == 0);
    print_array(TABLE_SIZE, parent);

    FindReplace(TABLE_SIZE, parent, 2, 3);
    print_array(TABLE_SIZE, parent);

    FindReplace(TABLE_SIZE, parent, 3, 4);
    print_array(TABLE_SIZE, parent);

    FindReplace(TABLE_SIZE, parent, 3, 5);
    print_array(TABLE_SIZE, parent);

    FindReplace(TABLE_SIZE, parent, 8, 7);
    print_array(TABLE_SIZE, parent);

    FindReplace(TABLE_SIZE, parent, 7, 9);
    print_array(TABLE_SIZE, parent);

    assert(Find(TABLE_SIZE, parent, 3) == 5);
    assert(Find(TABLE_SIZE, parent, 7) == 9);

    FindCompress(TABLE_SIZE, parent, 2);
    print_array(TABLE_SIZE, parent);

    FindReplace(TABLE_SIZE, parent, 0, 2);
    print_array(TABLE_SIZE, parent);

    FindReplace(TABLE_SIZE, parent, 1, 7);
    print_array(TABLE_SIZE, parent);

    FindCompress(TABLE_SIZE, parent, 8);
    print_array(TABLE_SIZE, parent);

    Union(TABLE_SIZE, parent, 0, 1);
    print_array(TABLE_SIZE, parent);

    assert(Find(TABLE_SIZE, parent, 1) == Find(TABLE_SIZE, parent, 3));
    assert(Find(TABLE_SIZE, parent, 1) != Find(TABLE_SIZE, parent, 6));
#else
    test_table();
#endif

    return 0;
}