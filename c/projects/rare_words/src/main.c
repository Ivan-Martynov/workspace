#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

#include "../include/string_helper.h"
#include "../include/file_line_reader.h"
#include "../include/printmode_wide.h"
#include "../include/rare_word_wide.h"

struct rare_word_collection
{
    struct rare_word_wide* array;
    size_t size;
    size_t capacity;
};

struct rare_word_collection make_empty_collection(const size_t capacity)
{
    struct rare_word_collection entries = {
        .array = NULL,
        .size = 0,
        .capacity = capacity
    };

    entries.array = malloc(entries.capacity * sizeof(*entries.array));

    return entries;
}

void free_collection(struct rare_word_collection* entries_ptr)
{
  for (size_t i = 0; i < entries_ptr->size; ++i)
  {
    free_rare_word_wide(&entries_ptr->array[i]);
  }
  free(entries_ptr->array);
  entries_ptr->size = 0;
}

bool add_empty_entry(struct rare_word_collection* entries_ptr)
{
    if (entries_ptr->size >= entries_ptr->capacity)
    {
      entries_ptr->capacity <<= 1;
      entries_ptr->array = realloc(entries_ptr->array, entries_ptr->capacity * sizeof(*entries_ptr->array));

      if (entries_ptr->array == NULL)
      {
          return false;
      }
    }
    ++entries_ptr->size;
    //entries_ptr->array[entries_ptr->size++] = make_empty_entry();

    return true;
}

bool insert_entry_at(struct rare_word_collection *entries_ptr, const size_t idx,
                     struct rare_word_wide entry)
{
    if ((idx <= entries_ptr->size) && add_empty_entry(entries_ptr))
    {
        for (size_t i = entries_ptr->size - 1; i > idx; --i)
        {
            entries_ptr->array[i] = entries_ptr->array[i - 1];
        }

        entries_ptr->array[idx] = entry;

        return true;
    }
    else
    {
        return false;
    }
}

void display_collection(const struct rare_word_collection* const entries_ptr)
{
    for (size_t i = 0; i < entries_ptr->size; ++i)
    {
        display_rare_word_wide(&entries_ptr->array[i]);
    }
}

void process_line(struct rare_word_collection* entries_ptr, const wchar_t* const line)
{
  wchar_t* title = find_title(line);

  if (title != NULL)
  {
    add_empty_entry(entries_ptr);
    allocate_and_copy_wide(&entries_ptr->array[entries_ptr->size - 1].title, title);
  }

  if ((entries_ptr->size > 0) && (entries_ptr->array[entries_ptr->size - 1].title != NULL))
  {
    struct rare_word_wide* word_ptr = &entries_ptr->array[entries_ptr->size - 1];

    try_add_wide_part_of_speech(word_ptr, line);
    try_add_wide_origin(word_ptr, line);
    try_add_wide_description(word_ptr, line);
    try_add_wide_example(word_ptr, line);
  }
}

void fill_from_stream(struct rare_word_collection* restrict entries_ptr,
                      FILE* restrict stream)
{
    wchar_t *line = NULL;
    while ((line = getline_from_file_wide(stream)) != NULL)
    {
        process_line(entries_ptr, line);
    }
    free(line);
    fclose(stream);
}

void list_titles(const struct rare_word_collection* const entries)
{
    for (size_t i = 0; i < entries->size; ++i)
    {
        wprintf(L"%d: %ls\n", i, entries->array[i].title);
    }
}

void test_reading_from_file(const char* const filepath)
{
    setlocale(LC_ALL, "en_US.UTF-8");

    FILE* file_ptr = fopen(filepath, "r, css=UTF-8");

    if (file_ptr == NULL)
    {
        wprintf(L"Unable to open file %s: %s\n", filepath, strerror(errno));
    }
    else
    {
        struct rare_word_collection entries = make_empty_collection(4);

        fill_from_stream(&entries, file_ptr);

        //struct rare_word_wide word = { NULL };
        //insert_entry_at(&entries, 0, word);

        list_titles(&entries);

        display_collection(&entries);

        free_collection(&entries);
    }
}

int main()
{
    test_reading_from_file("entries_default.txt");

    return 0;
}