#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

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
        size_t buf_len = wcslen(buffer);

        if (len > SIZE_MAX - buf_len)
        {
            if (line != NULL)
            {
                free(line);
            }

            return NULL;
        }
        else
        {
            len += buf_len + 1;
        }

        if ((line = realloc(line, len * sizeof(*line))) == NULL)
        {
            return NULL;
        }

        wcscat(line, buffer);
        line[len] = L'\0';

        if (buffer[buf_len - 1] == L'\n')
        {
            return line;
        }
    }

    return NULL;
}

size_t getline_from_file(char** restrict line, size_t* restrict len,
                         FILE* restrict file_p)
{
    if ((line == NULL) || (len == NULL) || (file_p == NULL))
    {
        errno = EINVAL;
        return -1;
    }

    char buffer[CHUNK_SIZE];

    if ((*line == NULL) || (*len < sizeof(buffer)))
    {
        *len = sizeof(buffer);

        if ((*line = malloc(*len)) == NULL)
        {
            errno = ENOMEM;
            return -1;
        }
    }

    (*line)[0] = '\0';

    while (fgets(buffer, sizeof(buffer), file_p) != NULL)
    {
        size_t len_used = strlen(*line);
        size_t buf_len = strlen(buffer);

        if (*len - len_used < buf_len)
        {
            if (*len > SIZE_MAX >> 1)
            {
                errno = EOVERFLOW;
                return -1;
            }
            else
            {
                (*len) <<= 1;
            }
        }

        if ((*line = realloc(*line, (*len) * sizeof(**line))) == NULL)
        {
            errno = ENOMEM;
            return -1;
        }

        strcat(*line, buffer);
        len_used += buf_len;
        (*line)[len_used] = '\0';

        if ((*line)[len_used - 1] == '\n')
        {
            return len_used;
        }
    }

    return -1;
}

int getwline_from_file(wchar_t** restrict line, size_t* len, FILE* file_p)
{
    if ((line == NULL) || (len == NULL) || (file_p == NULL))
    {
        errno = EINVAL;
        return -1;
    }

    wchar_t buffer[CHUNK_SIZE];

    if ((*line == NULL) || (*len < CHUNK_SIZE))
    {
        *len = CHUNK_SIZE;

        if ((*line = malloc((*len) * sizeof(**line))) == NULL)
        {
            errno = ENOMEM;
            return -1;
        }
    }

    (*line)[0] = L'\0';

    while (fgetws(buffer, CHUNK_SIZE, file_p) != NULL)
    {
        size_t len_used = wcslen(*line);
        size_t buf_len = wcslen(buffer);

        if (*len - len_used < buf_len)
        {
            if (*len > SIZE_MAX >> 1)
            {
                errno = EOVERFLOW;
                return -1;
            }
            else
            {
                (*len) <<= 1;
            }
        }

        if ((*line = realloc(*line, (*len) * sizeof(**line))) == NULL)
        {
            errno = ENOMEM;
            return -1;
        }

        wcscat(*line, buffer);
        len_used += buf_len;
        (*line)[len_used] = L'\0';

        if ((*line)[len_used - 1] == L'\n')
        {
            return len_used;
        }
    }

    return -1;
}

void wread_from_file(const char* filepath)
{
    setlocale(LC_ALL, "");

    //FILE* file_p = fopen(filepath, "r, css=UTF-8");
    FILE* file_p = fopen(filepath, "r");

    if (file_p == NULL)
    {
        printf("Unable to open file %s: %s\n", filepath, strerror(errno));
        return;
    }

    wchar_t* line = NULL;
    size_t len = 0;

    while ((line = wgetline_from_file(file_p)) != NULL)
    //while (getwline_from_file(&line, &len, file_p) != -1)
    {
        wprintf(L"Line: %ls", line);
    }

    if (line != NULL)
    {
        free(line);
    }

    fclose(file_p);
}

void read_from_file(const char* filepath)
{
    FILE* file_p = fopen(filepath, "r");

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

    if (line != NULL)
    {
        free(line);
    }

    fclose(file_p);
}

int main(int argc, char *agrv[])
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

  //read_from_file("words.txt");
  wread_from_file("words.txt");

  return 0;
}