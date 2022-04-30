#include "../include/string_helper.h"

#include <stdlib.h>
#include <string.h>

bool nullify_char_wide(wchar_t* const line, const wchar_t ch)
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

bool allocate_and_copy_wide(wchar_t** restrict dest, const wchar_t* restrict const src)
{
    if ((*dest = malloc((wcslen(src) + 1) * sizeof(**dest))) != NULL)
    {
        wcscpy(*dest, src);
        return true;
    }
    else
    {
        return false;
    }
}

bool reallocate_and_cat_wide(wchar_t** restrict dest, const wchar_t* restrict const src)
{
    size_t src_len = wcslen(src) + 1;
    size_t dest_len = (*dest == NULL) ? 0 : wcslen(*dest);

    if ((*dest = realloc(*dest, (dest_len + src_len ) * sizeof(**dest))) != NULL)
    {
        wcscat(*dest, src);
        return true;
    }
    else
    {
        return false;
    }
}
