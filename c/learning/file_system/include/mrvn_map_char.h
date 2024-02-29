#ifndef _H_MRVN_MAP_CHAR_H_
#define _H_MRVN_MAP_CHAR_H_

#include <stddef.h>
#include <stdbool.h>

struct mrvn_pair_char;
struct mrvn_map_char;

struct mrvn_map_char* mrvn_map_char_new(const size_t, const size_t);

void mrvn_map_char_delete(struct mrvn_map_char*);

size_t mrvn_map_char_capacity(const struct mrvn_map_char* const);
size_t mrvn_map_char_size(const struct mrvn_map_char* const);

struct mrvn_pair_char** mrvn_map_char_get_pairs_ptr(
    const struct mrvn_map_char* const);

const char* mrvn_map_char_get_key_at(
    const struct mrvn_map_char* const, const size_t);
const char* mrvn_map_char_get_value_at(
    const struct mrvn_map_char* const, const size_t);

const char* mrvn_map_char_get_value(
    const struct mrvn_map_char* const, const char* const);

void mrvn_map_char_remove_at(struct mrvn_map_char* const, const size_t);
void mrvn_map_char_remove(struct mrvn_map_char* const, const char[static 1]);

bool mrvn_map_char_resize(struct mrvn_map_char* const, const size_t);
bool mrvn_map_char_resize_default(struct mrvn_map_char* const);

struct mrvn_pair_char* mrvn_map_char_insert(
    struct mrvn_map_char* const, const char[static 1], const char[static 1]);

#endif //_H_MRVN_MAP_CHAR_H_
