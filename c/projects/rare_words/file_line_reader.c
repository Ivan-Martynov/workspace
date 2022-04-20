#include "file_line_reader.h"
#include "string_helper.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define CHUNK_SIZE 16

long int getline_from_file(char** restrict line, size_t* restrict len,
                         FILE* restrict file_p)
{
    if ((line == NULL) || (len == NULL) || (file_p == NULL))
    {
        return -1;
    }

    char buffer[CHUNK_SIZE];

    if ((*line == NULL) || (*len < sizeof(buffer)))
    {
        *len = sizeof(buffer);

        if ((*line = malloc(*len)) == NULL)
        {
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
                return -1;
            }
            else
            {
                (*len) <<= 1;
            }
        }

        if ((*line = realloc(*line, (*len) * sizeof(**line))) == NULL)
        {
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

long int
getwline_from_file(wchar_t** restrict line, size_t* restrict len, FILE* restrict file_ptr)
{
    if ((line == NULL) || (len == NULL) || (file_ptr == NULL))
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

    while (fgetws(buffer, CHUNK_SIZE, file_ptr) != NULL)
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