#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

#include "string_helper.h"
#include "file_line_reader.h"
#include "printmode_wide.h"

#define CHUNK_SIZE 16

struct rare_word_wide
{
    wchar_t* title;
    wchar_t* part_of_speech;
    wchar_t* origin;
    wchar_t* description;

    wchar_t** examples;
    size_t example_count;
};

struct rare_word_collection
{
    struct rare_word_wide* array;
    size_t size;
    size_t capacity;
};

void free_rare_word_wide(struct rare_word_wide* entry)
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

void init_collection(struct rare_word_collection* entries)
{
    entries->array = NULL;
    entries->capacity = 1;
    entries->size = 0;
    entries->array = malloc(entries->capacity * sizeof(*entries->array));
}

void free_collection(struct rare_word_collection* entries)
{
  for (size_t i = 0; i < entries->size; ++i)
  {
    free_rare_word_wide(&entries->array[i]);
  }
  free(entries->array);
  entries->size = 0;
}

bool add_entry(struct rare_word_collection* entries)
{
    if (entries->size >= entries->capacity)
    {
      entries->capacity <<= 1;
      entries->array = realloc(entries->array, entries->capacity * sizeof(*entries->array));

      if (entries->array == NULL)
      {
          return false;
      }
    }

    struct rare_word_wide entry = { NULL };
    entries->array[entries->size++] = entry;

    return true;
}

void display_component(const wchar_t *const component,
                       const wchar_t *const text,
                       const enum color_code background_color,
                       const enum color_code foreground_color)
{
  if (text != NULL)
  {
    set_printmode_bg(background_color);
    set_printmode_fg(foreground_color);
    wprintf(component);
    set_printmode_fgh(foreground_color);
    wprintf(L"%ls", text);
    reset_printmode();
    wprintf(L"\n");
  }
}

void display_rare_word_wide(const struct rare_word_wide* const word)
{
  wprintf(L"\n");
  display_component(L"Title: ", word->title, YELLOW_C, BLUE_C);
  display_component(L"Part of speech: ", word->part_of_speech, CYAN_C, RED_C);
  display_component(L"Description: ", word->description, RED_C, CYAN_C);
  display_component(L"Origin: ", word->origin, GREEN_C, MAGENTA_C);

  for (size_t i = 0; i < word->example_count; ++i) {
    display_component(L"Example: ", word->examples[i], MAGENTA_C, GREEN_C);
  }
}

void display_collection(const struct rare_word_collection* const entries)
{
    for (size_t i = 0; i < entries->size; ++i)
    {
        display_rare_word_wide(&entries->array[i]);
    }
}

wchar_t* find_title(const wchar_t* const line)
{
    const wchar_t text[] = L"Title: ";
    wchar_t* res = wcsstr(line, text);

    return (res == line) ? (res + wcslen(text)) : NULL;
}

bool try_add_wide_title(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Title: ";
    return (wcsstr(line, text) == line)
        ? wallocate_and_copy(&entry->title, line + wcslen(text)) : false;
}

bool try_add_wide_part_of_speech(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Part of speech: ";
    return (wcsstr(line, text) == line)
        ? wallocate_and_copy(&entry->part_of_speech, line + wcslen(text)) : false;
}

bool try_add_wide_origin(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Origin: ";
    return (wcsstr(line, text) == line)
        ? wallocate_and_copy(&entry->origin, line + wcslen(text)) : false;
}

bool try_add_wide_description(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Description: ";
    return (wcsstr(line, text) == line)
        ? wallocate_and_copy(&entry->description, line + wcslen(text)) : false;
}

bool try_add_wide_example(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Example: ";
    bool valid = (wcsstr(line, text) == line);

    if (valid)
    {
        size_t n = entry->example_count + 1;
        valid = ((entry->examples = realloc(entry->examples, n * sizeof(*entry->examples))) != NULL);

        if (valid)
        {
          valid = wallocate_and_copy(&entry->examples[entry->example_count],
                             line + wcslen(text));
        }

        entry->example_count = valid ? n : 0;
    }

    return valid;
}

wchar_t* wgetline_from_file(FILE* restrict stream)
{
    wchar_t* line = NULL;

    wchar_t buffer[CHUNK_SIZE];

    while (fgetws(buffer, CHUNK_SIZE, stream) != NULL)
    {
        wreallocate_and_cat(&line, buffer);
        if (nullify_wchar(line, L'\n'))
        {
          nullify_wchar(line, L'\r');
          break;
        }
    }

    return line;
}

void process_line(struct rare_word_collection* entries, const wchar_t* const line)
{
  wchar_t* title = find_title(line);

  if (title != NULL)
  {
    add_entry(entries);
    wallocate_and_copy(&entries->array[entries->size - 1].title, title);
  }

  if ((entries->size > 0) && (entries->array[entries->size - 1].title != NULL))
  {
    struct rare_word_wide* word_ptr = &entries->array[entries->size - 1];

    try_add_wide_part_of_speech(word_ptr, line);
    try_add_wide_origin(word_ptr, line);
    try_add_wide_description(word_ptr, line);
    try_add_wide_example(word_ptr, line);
  }
}

void read_from_file(const char* filepath)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    // Can be "" for the user-preferred locale or "C" for the minimal locale
    //setlocale(LC_ALL, "");

    FILE* file_p = fopen(filepath, "r, css=UTF-8");

    if (file_p == NULL)
    {
        wprintf(L"Unable to open file %s: %s\n", filepath, strerror(errno));
        return;
    }

    struct rare_word_collection entries = { .array = NULL, .capacity = 32, .size = 0 };
    entries.array = malloc(entries.capacity * sizeof(*entries.array));

    wchar_t* line = NULL;
    while ((line = wgetline_from_file(file_p)) != NULL)
    {
        process_line(&entries, line);
    }
    free(line);
    fclose(file_p);

    display_collection(&entries);
    free_collection(&entries);
}

int main()
{
  read_from_file("entries_default.txt");

  return 0;
}