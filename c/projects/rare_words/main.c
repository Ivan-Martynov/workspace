#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

#include "string_helper.h"
#include "file_line_reader.h"

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

struct rare_word
{
    char* term;
    char* part_of_speech;
    char* origin;
    char* description;
};

void display_rare_word_wide(const struct rare_word_wide* const word)
{
    wprintf(L"%ls.\n", word->title);
    wprintf(L"%ls\n", word->part_of_speech);
    wprintf(L"%ls\n", word->description);
    wprintf(L"Origin: %ls\n", word->origin);
}

void display_rare_word(const struct rare_word word)
{
    printf("%s.\n", word.term);
    printf("%s\n", word.part_of_speech);
    printf("%s\n", word.description);
    printf("Origin: %s\n", word.origin);
}

void free_char_pointer(char* p)
{
    if (p != NULL)
    {
        free(p);
    }
}

void free_rare_word(struct rare_word* word)
{
    free_char_pointer(word->term);
    //free_char_pointer(word->part_of_speech);
    free_char_pointer(word->origin);
    free_char_pointer(word->description);
}

bool try_add_wide_title(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Title: ";
    if (wcsstr(line, text) == line)
    {
        wallocate_and_copy(&entry->title, line + wcslen(text));
        wprintf(L"%ls\n", entry->title);
        return true;
    }
    else
    {
        return false;
    }
}

bool try_add_wide_part_of_speech(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Part of speech: ";
    if (wcsstr(line, text) == line)
    {
        wallocate_and_copy(&entry->part_of_speech, line + wcslen(text));
        wprintf(L"%ls\n", entry->part_of_speech);
        return true;
    }
    else
    {
        return false;
    }
}

bool try_add_wide_origin(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Origin: ";
    if (wcsstr(line, text) == line)
    {
        wallocate_and_copy(&entry->origin, line + wcslen(text));
        wprintf(L"%ls\n", entry->origin);
        return true;
    }
    else
    {
        return false;
    }
}

bool try_add_wide_description(struct rare_word_wide* entry, const wchar_t* const line)
{
    const wchar_t text[] = L"Description: ";
    if (wcsstr(line, text) == line)
    {
        wallocate_and_copy(&entry->description, line + wcslen(text));
        wprintf(L"%ls\n", entry->description);
        return true;
    }
    else
    {
        return false;
    }
}


void add_wide_title(struct rare_word_wide* rw, const wchar_t* const title)
{
    wallocate_and_copy(&rw->title, title);

}

struct rare_word make_word(
    const char* term,
    const char* origin,
    const char* description
    )
{
    struct rare_word result = { NULL };

    allocate_and_copy(&result.term, term);
    allocate_and_copy(&result.origin, origin);
    //allocate_and_copy(&result.description, description);
    reallocate_and_cat(&result.description, description);

    return result;
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
          return line;
        }
    }

    return line;
}

void process_wline(struct rare_word_wide* entry, const wchar_t* const line)
{
    try_add_wide_title(entry, line);
    try_add_wide_part_of_speech(entry, line);
    try_add_wide_origin(entry, line);
    try_add_wide_description(entry, line);

    //display_rare_word_wide(entry);
}

void wread_from_file(const char* filepath)
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

    wchar_t* line = NULL;
    //size_t len = 0;

    struct rare_word_wide entry = { NULL };

    while ((line = wgetline_from_file(file_p)) != NULL)
    //while (getwline_from_file(&line, &len, file_p) != -1)
    {
        process_wline(&entry, line);
#if 0
        const wchar_t text[] = L"Title: ";
        wchar_t* res = wcsstr(line, text);
        //wchar_t* res = wcsstr(line, L"Accismus");
        if (res == line)
        {
            add_wide_title(&entry, line + wcslen(text));
            wprintf(L"Line: %ls: %zu\n", line, wcslen(line));
            wprintf(L"Try: %ls\n", entry.title);
            //wprintf(L"Search result: %ls %d\n", res, res - line);
        }
#endif
    }
    //wprintf(L"Line: %ls\n", line);

    if (line != NULL)
    {
        free(line);
    }

    fclose(file_p);
}

void read_from_file(const char* filepath)
{
    FILE* file_p = fopen(filepath, "r");
    fwide(file_p, 1);

    if (file_p == NULL)
    {
        printf("Unable to open file %s: %s\n", filepath, strerror(errno));
        return;
    }

    char* line = NULL;
    size_t len = 0;

    while (getline_from_file(&line, &len, file_p) != -1)
    {
        printf("Line: %s", line);
    }
    printf("Line: %s", line);

    if (line != NULL)
    {
        free(line);
    }

    fclose(file_p);
}

int main()
{
#if 0
    const char* term = "Accismus";
    const char* description = "Accismus is a useful term for pretending to be "
                       "disinterested in something when you actually want it. "
                       "Pull this word out when you see someone acting like "
                       "he doesn’t want the last donut.";
    const char* origin = "Via Latin accismus from Ancient Greek ἀκκισμός "
                  "(akkismos, “prudery”)";

    struct rare_word word = make_word(term, origin, description);
    word.part_of_speech = "Noun";

    display_rare_word(word);

    free_rare_word(&word);
#endif

#if 1
  //read_from_file("words.txt");
  wread_from_file("words.txt");
#endif

    // getchar();

    return 0;
}