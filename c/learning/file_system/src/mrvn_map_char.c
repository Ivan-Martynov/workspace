#include "mrvn_map_char.h"
#include "mrvn_string_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const size_t mrvn_map_default_capacity_step = 8;

struct mrvn_pair_char
{
    char* key;
    char* value;
};

struct mrvn_map_char
{
    struct mrvn_pair_char* pairs;
    size_t size;
    size_t capacity;
};

struct mrvn_pair_char* mrvn_map_char_pair_init(
    struct mrvn_pair_char* const pair_ptr, const char key[static 1],
    const char value[static 1])
{
    if (pair_ptr)
    {
        mrvn_allocate_and_copy_char(&pair_ptr->key, key);
        mrvn_allocate_and_copy_char(&pair_ptr->value, value);
    }

    return pair_ptr;
}

struct mrvn_pair_char* mrvn_map_char_pair_new(
    const char key[static 1], const char value[static 1])
{
    return mrvn_map_char_pair_init(
        malloc(sizeof(struct mrvn_pair_char)), key, value);
}

void mrvn_map_char_pair_delete(struct mrvn_pair_char* pair_ptr)
{
    if (pair_ptr)
    {
        free(pair_ptr->key);
        free(pair_ptr->value);
        free(pair_ptr);
    }
}

struct mrvn_pair_char mrvn_map_char_pair_make(
    const char key[static 1], const char value[static 1])
{
    struct mrvn_pair_char pair = {0};
    mrvn_map_char_pair_init(&pair, key, value);

    return pair;
}

struct mrvn_map_char* mrvn_map_char_init(
    struct mrvn_map_char* map_ptr, const size_t capacity, const size_t size)
{
    if (map_ptr)
    {
        map_ptr->capacity = capacity;
        map_ptr->size = size;
        map_ptr->pairs = NULL;
        if ((capacity > 0) && !mrvn_map_char_resize(map_ptr, capacity))
        {
            return NULL;
        }
    }

    return map_ptr;
}

struct mrvn_map_char* mrvn_map_char_new(
    const size_t capacity, const size_t size)
{
    return mrvn_map_char_init(
        malloc(sizeof(struct mrvn_map_char)), capacity, size);
}

void mrvn_map_char_delete(struct mrvn_map_char* map_ptr)
{
    if (map_ptr)
    {
        for (size_t i = 0; i < map_ptr->size; ++i)
        {
            free(map_ptr->pairs[i].key);
            free(map_ptr->pairs[i].value);
        }

        free(map_ptr->pairs);
        free(map_ptr);
    }
}

size_t mrvn_map_char_capacity(const struct mrvn_map_char* const map_ptr)
{
    return map_ptr->capacity;
}

size_t mrvn_map_char_size(const struct mrvn_map_char* const map_ptr)
{
    return map_ptr->size;
}

bool mrvn_map_char_resize(
    struct mrvn_map_char* const map_ptr, const size_t size)
{
    map_ptr->pairs = realloc(map_ptr->pairs, size * sizeof(*(map_ptr->pairs)));
    if (map_ptr->pairs)
    {
        map_ptr->capacity = size;
        return true;
    }
    else
    {
        return false;
    }
}

bool mrvn_map_char_resize_default(struct mrvn_map_char* const map_ptr)
{
    const size_t size = map_ptr->capacity + mrvn_map_default_capacity_step;
    return mrvn_map_char_resize(
        map_ptr, (size > map_ptr->size) ? size : map_ptr->size + 1);
}

const struct mrvn_pair_char* mrvn_map_char_get_pairs_ptr(
    const struct mrvn_map_char* const map_ptr)
{
    return map_ptr->pairs;
}

const struct mrvn_pair_char* mrvn_map_char_get_at(
    const struct mrvn_map_char* const map_ptr, const size_t i)
{
    return (i < map_ptr->size) ? &map_ptr->pairs[i] : NULL;
}

const char* mrvn_map_char_get_key_at(
    const struct mrvn_map_char* const map_ptr, const size_t i)
{
    const struct mrvn_pair_char* const pair_ptr
        = mrvn_map_char_get_at(map_ptr, i);
    return pair_ptr ? pair_ptr->key : NULL;
}

const char* mrvn_map_char_get_value_at(
    const struct mrvn_map_char* const map_ptr, const size_t i)
{
    const struct mrvn_pair_char* const pair_ptr
        = mrvn_map_char_get_at(map_ptr, i);
    return pair_ptr ? pair_ptr->value : NULL;
}

const char* mrvn_map_char_get_value(
    const struct mrvn_map_char* const map_ptr, const char* const key)
{
    for (size_t i = 0; i < map_ptr->size; ++i)
    {
        if (strcmp(map_ptr->pairs[i].key, key) == 0)
        {
            return map_ptr->pairs[i].value;
        }
    }

    return NULL;
}

static long long get_index_to_insert_at(
    struct mrvn_map_char* const map_ptr, const char key[static 1])
{
    long long left = 0;
    long long right = map_ptr->size - 1;
    while (left <= right)
    {
        const long long middle = left + ((right - left) >> 1);

        const char* const current_key
            = mrvn_map_char_get_key_at(map_ptr, middle);

        const int compare_result = strcmp(key, current_key);
        if (compare_result < 0)
        {
            right = middle - 1;
        }
        else if (compare_result > 0)
        {
            left = middle + 1;
        }
        else
        {
            return -1;
        }
    }

    return left;
}

bool mrvn_map_char_insert(struct mrvn_map_char* const map_ptr,
    const char key[static 1], const char value[static 1])
{
    if ((map_ptr->size >= map_ptr->capacity)
        && !mrvn_map_char_resize_default(map_ptr))
    {
        return false;
    }

    const long long index = get_index_to_insert_at(map_ptr, key);
    if (index == -1)
    {
        return false;
    }

    // Copy (shift) elements on the right to place the new pair.
    for (long long i = map_ptr->size; i > index; --i)
    {
        map_ptr->pairs[i] = map_ptr->pairs[i - 1];
    }
    mrvn_map_char_pair_init(&(map_ptr->pairs[index]), key, value);
    ++map_ptr->size;

    return true;
}

void mrvn_map_char_delete_at(
    struct mrvn_map_char* const map_ptr, const size_t index)
{
    if (index >= map_ptr->size)
    {
        return;
    }

    for (size_t i = index; i < map_ptr->size; ++i)
    {
        map_ptr->pairs[i] = map_ptr->pairs[i + 1];
    }

    mrvn_map_char_pair_delete(&map_ptr->pairs[--map_ptr->size]);
}

//void mrvn_map_char_delete(
//    struct mrvn_map_char* const map_ptr, const char key[static 1])
//{
//}
