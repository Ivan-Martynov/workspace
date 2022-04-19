#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

#include "file_line_reader.h"

#define CHUNK_SIZE 16

struct rare_word_wide
{
    wchar_t* title;
    wchar_t* part_of_speech;
    wchar_t* origin;
    wchar_t* description;

};

struct rare_word
{
    char* term;
    char* part_of_speech;
    char* origin;
    char* description;
};

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

void allocate_and_copy(char** dest, const char* src)
{
    size_t len = strlen(src) + 1;
    *dest = malloc(sizeof(*dest) * len);

    if (*dest != NULL)
    {
        strcpy(*dest, src);
    }
}

struct rare_word make_word(
    const char* term,
    const char* origin,
    const char* description
    )
{
    struct rare_word result;

    allocate_and_copy(&result.term, term);
    allocate_and_copy(&result.origin, origin);
    allocate_and_copy(&result.description, description);

    return result;
}

wchar_t* wgetline_from_file(FILE* restrict stream)
{
    wchar_t* line = NULL;

    size_t len = 0;

    wchar_t buffer[CHUNK_SIZE];

    while (fgetws(buffer, CHUNK_SIZE, stream) != NULL)
    {
        size_t buf_len = wcslen(buffer) + 1;

        if (buffer[buf_len - 1] == L'\0')
        {
            //wprintf(L"Doing good");
        }

        //wprintf(L"Buffer size: %zu\n, %C\n", buf_len, buffer[buf_len - 2]);

        if (len > SIZE_MAX - CHUNK_SIZE)
        {
            if (line != NULL)
            {
                free(line);
            }

            return NULL;
        }
        else
        {
            len += CHUNK_SIZE;
        }

        wchar_t* temp_buf = realloc(line, len * sizeof(*line));
        if (temp_buf == NULL)
        //if ((line = realloc(line, len * sizeof(*line))) == NULL)
        {
            if (line != NULL)
            {
                free(line);
            }

            return NULL;
        }
        else
        {
            line = temp_buf;
        }

        //wcscat(line, buffer);
        memcpy(line + wcslen(line), buffer, buf_len * sizeof(*line));
        line[len] = L'\0';

        if (line != NULL)
        {
          wprintf(L"Line size: %zu, %zu\n", wcslen(line), buf_len);
        }

        if (buffer[buf_len - 2] == L'\n')
        //if (line[len - 1] == L'\n')
        {
            return line;
        }
    }

    //return NULL;
    return line;
}

void wread_from_file(const char* filepath)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    // Can be "" for the user-preferred locale or "C" for the minimal locale
    //setlocale(LC_ALL, "");

    FILE* file_p = fopen(filepath, "r, css=UTF-8");
    //FILE* file_p = fopen(filepath, "r");
    //fwide(file_p, -1);

    if (file_p == NULL)
    {
        printf("Unable to open file %s: %s\n", filepath, strerror(errno));
        return;
    }

    wchar_t* line = NULL;
    size_t len = 0;

    while (getwline_from_file(&line, &len, file_p) != -1)
    {
      wprintf(L"Line: %ls\n", line);
    }
    wprintf(L"Line: %ls\n", line);

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

  read_from_file("words.txt");
  wread_from_file("words.txt");

  //getchar();

  return 0;
}