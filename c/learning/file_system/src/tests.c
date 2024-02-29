#include "mrvn_map_char.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void print_map_char(const struct mrvn_map_char* const map_ptr)
{
    const size_t map_size = mrvn_map_char_size(map_ptr);
    if (map_size > 0)
    {
        printf("Printing map (%zu elements):\n", map_size);
    }
    else
    {
        printf("Map is empty.\n");
    }

    for (size_t i = 0; i < map_size; ++i)
    {
        printf("%zu: [%s] = %s\n", i, mrvn_map_char_get_key_at(map_ptr, i),
            mrvn_map_char_get_value_at(map_ptr, i));
        // printf("Key/value at %zu = %s/%s\n", i,
        //    mrvn_map_char_get_key_at(map_ptr, i),
        //    mrvn_map_char_get_value_at(map_ptr, i));
    }
}

void test_1()
{
    const char* keys[] = {"B", "D", "F", "K", "M", "R"};
    const char* values[]
        = {"Value B", "Value D", "Value F", "Value K", "Value M", "Value R"};

    const size_t keys_size = sizeof(keys) / sizeof(*keys);
    const size_t values_size = sizeof(values) / sizeof(*values);
    const size_t size = (keys_size >= values_size) ? values_size : keys_size;

    const size_t capacity = 0;
    struct mrvn_map_char* map_ptr = mrvn_map_char_new(capacity, 0);

    assert(mrvn_map_char_size(map_ptr) == 0);
    assert(mrvn_map_char_capacity(map_ptr) == capacity);

    if (!map_ptr)
    {
        printf("Failed to create a map.\n");
        return;
    }

    printf("Created map with capacity %zu and size %zu\n",
        mrvn_map_char_capacity(map_ptr), mrvn_map_char_size(map_ptr));

    print_map_char(map_ptr);

    for (size_t i = 0; i < size; ++i)
    {
        mrvn_map_char_insert(map_ptr, keys[i], values[i]);
    }

    assert(mrvn_map_char_size(map_ptr) == size);

    mrvn_map_char_insert(map_ptr, "E", "Value E");
    mrvn_map_char_insert(map_ptr, "A", "Value A");
    mrvn_map_char_insert(map_ptr, "S", "Value S");
    mrvn_map_char_insert(map_ptr, "R", "New value R");

    print_map_char(map_ptr);

    mrvn_map_char_remove_at(map_ptr, 4);
    mrvn_map_char_remove(map_ptr, "S");

    print_map_char(map_ptr);

    assert(!mrvn_map_char_get_value(map_ptr, "T"));
    assert(strcmp(mrvn_map_char_get_value(map_ptr, "A"), "Value A") == 0);

    printf("Removing elements.\n");
    for (int i = mrvn_map_char_size(map_ptr) - 1; i >= 0; --i)
    {
        printf("Removing at %d of %zu.\n", i, mrvn_map_char_size(map_ptr));
        mrvn_map_char_remove_at(map_ptr, i);
    }

    print_map_char(map_ptr);

    mrvn_map_char_delete(map_ptr);
}

void test_2()
{
    const struct
    {
        char* key;
        char* value;
    } temp_array[] = {{"TIT1", "Content group frame"}, {"TIT2", "Title"},
        {"TIT3", "Subtitle"}};

    const size_t n = sizeof(temp_array) / sizeof(*temp_array);
    struct mrvn_map_char* map_ptr = mrvn_map_char_new(n, 0);

    for (size_t i = 0; i < n; ++i)
    {
        mrvn_map_char_insert(map_ptr, temp_array[i].key, temp_array[i].value);
    }

    print_map_char(map_ptr);
    mrvn_map_char_delete(map_ptr);
}

int main()
{
    printf("Running tests.\n");
    test_1();
    test_2();

    return 0;
}
