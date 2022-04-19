#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

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

long int getwdelim_from_file(
    wchar_t** restrict line_ptr,
    size_t* restrict len,
    FILE* stream)
{

    if ((line_ptr == NULL) || (len == NULL))
    {
        return -1;
    }

    if ((*line_ptr == NULL) || (*len < CHUNK_SIZE))
    {
        *len = CHUNK_SIZE;
        if ((*line_ptr = malloc(CHUNK_SIZE * sizeof(**line_ptr))) == NULL)
        {
            return -1;
        }
    }

    (*line_ptr)[0] = L'\0';

    //long int current_len = 0;
    while (true)
    {
        if (ferror(stream))
        {
            return -1;
        }

        wint_t c = fgetwc(stream);
        if (c == WEOF)
        {
            break;
        }

        //size_t needed = current_len + 1;
    }

    return -1;
}

long long int getline_from_file(char** restrict line, size_t* restrict len,
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

long int getwline_from_file(wchar_t** restrict line, size_t* restrict len, FILE* restrict file_p)
{
    if ((line == NULL) || (len == NULL) || (file_p == NULL))
    {
        return -1;
    }

    const size_t element_size = sizeof(**line);

    wchar_t buffer[CHUNK_SIZE];

    if ((*line == NULL) || (*len < CHUNK_SIZE))
    {
        *len = CHUNK_SIZE;

        if ((*line = malloc((*len) * element_size)) == NULL)
        {
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
                return -1;
            }
            else
            {
                (*len) <<= 1;
            }
        }

        if ((*line = realloc(*line, (*len) * element_size)) == NULL)
        {
            return -1;
        }

        memcpy(*line + len_used, buffer, buf_len * element_size);
        len_used += buf_len;
        (*line)[len_used] = L'\0';

        if ((*line)[len_used - 1] == L'\n')
        {
          (*line)[len_used - 1] = L'\0';
          return len_used;
        }
    }

    return -1;
}

void scan_by_wchar(FILE* restrict stream)
{
    wint_t c;
    while((c = getwc(stream)) != WEOF)
    {
        putwchar(c);
    }
}

void wread_from_file(const char* filepath)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    // Can be "" for the user-preferred locale or "C" for the minimal locale
    //setlocale(LC_ALL, "");

    FILE* file_p = fopen(filepath, "r, css=UTF-8");
    //FILE* file_p = fopen(filepath, "r");

    if (file_p == NULL)
    {
        printf("Unable to open file %s: %s\n", filepath, strerror(errno));
        return;
    }

#if 0
    scan_by_wchar(file_p);
#else
    wchar_t* line = NULL;
    size_t len = 0;

    //while ((line = wgetline_from_file(file_p)) != NULL)
    while (getwline_from_file(&line, &len, file_p) != -1)
    {
      wprintf(L"Line: %ls\n", line);
    }

    if (line != NULL)
    {
        free(line);
    }
#endif

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

    while (getline(&line, &len, file_p) != -1)
    //while (getline_from_file(&line, &len, file_p) != -1)
    {
        printf("Line: %s", line);
    }

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

  //read_from_file("words.txt");
  wread_from_file("words.txt");

  //getchar();

  return 0;
}