#include "mrvn_map_char.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_1()
{
    const char* keys[] = {"B", "D", "F", "K", "M", "R"};
    const char* values[]
        = {"Value B", "Value D", "Value F", "Value K", "Value M", "Value R"};

    const size_t keys_size = sizeof(keys) / sizeof(*keys);
    const size_t values_size = sizeof(values) / sizeof(*values);
    const size_t size = keys_size >= values_size ? values_size : keys_size;

    const size_t capacity = 3;
    struct mrvn_map_char* map_ptr = mrvn_map_char_new(capacity, 0);

    assert(mrvn_map_char_size(map_ptr) == 0);
    assert(mrvn_map_char_capacity(map_ptr) == capacity);
    assert(mrvn_map_char_get_pairs_ptr(map_ptr));

    if (!map_ptr)
    {
        return;
    }

    printf("Created map with capacity %zu and size %zu\n",
        mrvn_map_char_capacity(map_ptr), mrvn_map_char_size(map_ptr));

    for (size_t i = 0; i < size; ++i)
    {
        mrvn_map_char_insert(map_ptr, keys[i], values[i]);
    }

    assert(mrvn_map_char_size(map_ptr) == size);

    mrvn_map_char_insert(map_ptr, "E", "Value E");
    mrvn_map_char_insert(map_ptr, "A", "Value A");
    mrvn_map_char_insert(map_ptr, "S", "Value S");

    mrvn_map_char_delete_at(map_ptr, 4);

    const size_t map_size = mrvn_map_char_size(map_ptr);
    for (size_t i = 0; i < map_size; ++i)
    {
        printf("Key/value at %zu = %s/%s\n", i,
            mrvn_map_char_get_key_at(map_ptr, i),
            mrvn_map_char_get_value_at(map_ptr, i));
    }

    assert(!mrvn_map_char_get_value(map_ptr, "T"));
    assert(strcmp(mrvn_map_char_get_value(map_ptr, "A"), "Value A") == 0);

    mrvn_map_char_delete(map_ptr);
}

int main()
{
    test_1();

    return 0;
}
