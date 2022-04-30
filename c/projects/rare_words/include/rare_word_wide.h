#ifndef RARE_WORD_WIDE_H
#define RARE_WORD_WIDE_H

#include <stdbool.h>
#include <wchar.h>

struct rare_word_wide
{
    wchar_t* title;
    wchar_t* part_of_speech;
    wchar_t* origin;
    wchar_t* description;

    wchar_t** examples;
    size_t example_count;
};

struct rare_word_wide make_empty_entry();

void free_rare_word_wide(struct rare_word_wide* entry);

void display_rare_word_wide(const struct rare_word_wide* const entry);

wchar_t* find_title(const wchar_t* const line);

bool try_add_wide_part_of_speech(struct rare_word_wide* entry,
                                 const wchar_t* const line);

bool try_add_wide_origin(struct rare_word_wide* entry,
                         const wchar_t* const line);

bool try_add_wide_description(struct rare_word_wide* entry,
                              const wchar_t* const line);

bool try_add_wide_example(struct rare_word_wide* entry,
                          const wchar_t* const line);

#endif // RARE_WORD_WIDE_H
