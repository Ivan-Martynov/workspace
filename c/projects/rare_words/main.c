#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

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

size_t getline_from_file(char** restrict line, size_t* restrict len, FILE* restrict file_p)
{
    if (line == NULL || len == 0 || file_p == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    char buffer[16];

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

        if ((*line = realloc(*line, *len)) == NULL)
        {
            errno = ENOMEM;
            return -1;
        }

        memcpy(*line + len_used, buffer, buf_len);
        len_used += buf_len;
        (*line)[len_used] = '\0';

        if ((*line)[len_used - 1] == '\n')
        {
            return len_used;
        }
    }

    return -1;
}

void read_from_file(const char* filepath)
{
    FILE* file_p = fopen(filepath, "r");

    if (file_p == NULL)
    {
        printf("Unable to open file %s\n", filepath);
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

#if 0
    char buffer[16];
    char* line = NULL;
    size_t len = 0;
    while (fgets(buffer, sizeof(buffer), file_p) != NULL)
    {
        if (feof(file_p))
        {
            break;
        }

        size_t buf_len = strlen(buffer);

        if (buf_len > 0)
        {
          len += buf_len;
          line = (char*)realloc(line, sizeof(*line) * len);
          if (line == NULL) {
            printf("Error realloc");
            break;
            }
            strcat(line, buffer);
        }

        if (buffer[buf_len - 1] == '\n')
        {
            if (len > 1)
            {
                printf("Line: %s", line);
            }
            free(line);
            line = NULL;
            len = 0;
        }
    }
#endif

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

  read_from_file("words.txt");

  return 0;
}