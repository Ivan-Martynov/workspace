#include "../include/rare_word_wide.h"
#include "../include/printmode_wide.h"
#include "../include/string_helper.h"

#include <stdlib.h>

struct rare_word_wide make_empty_entry()
{
    struct rare_word_wide entry = {
        .title = NULL,
        .part_of_speech = NULL,
        .origin = NULL,
        .description = NULL,
        .examples = NULL,
        .example_count = 0
    };

    return entry;
}

void free_rare_word_wide(struct rare_word_wide *entry)
{
    free(entry->title);
    free(entry->part_of_speech);
    free(entry->origin);
    free(entry->description);

    for (size_t i = 0; i < entry->example_count; ++i)
    {
        free(entry->examples[i]);
    }
    free(entry->examples);
    entry->example_count = 0;
}

void display_component(const wchar_t *const component,
                       const wchar_t *const text,
                       const enum color_code foreground_color)
{
    if (text != NULL)
    {
        set_printmode_fg(foreground_color);
        wprintf(component);
        set_printmode_fg(foreground_color);
        wprintf(L"%ls", text);
        reset_printmode();
        wprintf(L"\n");
    }
}

void display_rare_word_wide(const struct rare_word_wide *const entry)
{
    wprintf(L"\n");
    display_component(L"Title: ", entry->title, RED_C);
    display_component(L"Part of speech: ", entry->part_of_speech, BLUE_C);
    display_component(L"Description: ", entry->description, CYAN_C);
    display_component(L"Origin: ", entry->origin, MAGENTA_C);

    for (size_t i = 0; i < entry->example_count; ++i)
    {
        display_component(L"Example: ", entry->examples[i], GREEN_C);
    }
}

wchar_t *find_title(const wchar_t *const line)
{
    const wchar_t text[] = L"Title: ";
    wchar_t *res = wcsstr(line, text);

    return (res == line) ? (res + wcslen(text)) : NULL;
}

bool try_add_wide_title(struct rare_word_wide *entry,
                        const wchar_t *const line)
{
    const wchar_t text[] = L"Title: ";
    return (wcsstr(line, text) == line)
               ? allocate_and_copy_wide(&entry->title, line + wcslen(text))
               : false;
}

bool try_add_wide_part_of_speech(struct rare_word_wide *entry,
                                 const wchar_t *const line)
{
    const wchar_t text[] = L"Part of speech: ";
    return (wcsstr(line, text) == line)
               ? allocate_and_copy_wide(&entry->part_of_speech, line + wcslen(text))
               : false;
}

bool try_add_wide_origin(struct rare_word_wide *entry,
                         const wchar_t *const line)
{
    const wchar_t text[] = L"Origin: ";
    return (wcsstr(line, text) == line)
               ? allocate_and_copy_wide(&entry->origin, line + wcslen(text))
               : false;
}

bool try_add_wide_description(struct rare_word_wide *entry,
                              const wchar_t *const line)
{
    const wchar_t text[] = L"Description: ";
    return (wcsstr(line, text) == line)
               ? allocate_and_copy_wide(&entry->description, line + wcslen(text))
               : false;
}

bool try_add_wide_example(struct rare_word_wide *entry,
                          const wchar_t *const line)
{
    const wchar_t text[] = L"Example: ";
    bool valid = (wcsstr(line, text) == line);

    if (valid)
    {
        size_t n = entry->example_count + 1;
        valid = ((entry->examples =
                      realloc(entry->examples, n * sizeof(*entry->examples))) != NULL);

        if (valid)
        {
            valid = allocate_and_copy_wide(&entry->examples[entry->example_count],
                                       line + wcslen(text));
        }

        entry->example_count = valid ? n : 0;
    }

    return valid;
}
