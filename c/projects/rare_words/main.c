#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    *dest = malloc(sizeof(**dest) * len);

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

void read_from_file(const char* filepath)
{
    FILE* file_p = fopen(filepath, "r");

    char buffer[16];
    char* line = NULL;
    size_t len = 0;
    while (fgets(buffer, sizeof(buffer), file_p) != NULL)
    {
        len += strlen(buffer);

        if (len > 0)
        {
            line = realloc(line, sizeof(*line) * len);
            strcat(line, buffer);
        }

        if (buffer[strlen(buffer) - 1] == '\n')
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

    fclose(file_p);
}

int
main(int argc, char* agrv[])
{
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

    read_from_file("words.txt");

    return 0;
}