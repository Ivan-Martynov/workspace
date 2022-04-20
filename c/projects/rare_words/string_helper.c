#include "string_helper.h"

#include <stdlib.h>
#include <string.h>

bool nullify_char(char* const line, const char ch)
{
    char* res = strchr(line, ch);
    if (res != NULL)
    {
        line[res - line] = '\0';
        return true;
    }
    else
    {
        return false;
    }
}

bool nullify_wchar(wchar_t* const line, const wchar_t ch)
{
    wchar_t* res = wcschr(line, ch);
    if (res != NULL)
    {
        line[res - line] = L'\0';
        return true;
    }
    else
    {
        return false;
    }
}

void allocate_and_copy(char** restrict dest, const char* restrict const src)
{
    if ((*dest = malloc((strlen(src) + 1) * sizeof(**dest))) != NULL)
    {
        strcpy(*dest, src);
    }
}

void wallocate_and_copy(wchar_t** restrict dest, const wchar_t* restrict const src)
{
    if ((*dest = malloc((wcslen(src) + 1) * sizeof(**dest))) != NULL)
    {
        wcscpy(*dest, src);
    }
}

void reallocate_and_cat(char** restrict dest, const char* restrict const src)
{
  size_t src_len = strlen(src) + 1;
  size_t dest_len = (*dest == NULL) ? 0 : strlen(*dest);

  if ((*dest = realloc(*dest, (dest_len + src_len) * sizeof(**dest))) != NULL)
  {
    strcat(*dest, src);
  }
}

void wreallocate_and_cat(wchar_t** restrict dest, const wchar_t* restrict const src)
{
    size_t src_len = wcslen(src) + 1;
    size_t dest_len = (*dest == NULL) ? 0 : wcslen(*dest);

    if ((*dest = realloc(*dest, (dest_len + src_len ) * sizeof(**dest))) != NULL)
    {
        wcscat(*dest, src);
    }
}
