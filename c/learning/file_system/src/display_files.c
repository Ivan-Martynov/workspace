#define _GNU_SOURCE

#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "mrvn_string_helper.h"

#ifndef __STDC_ISO_10646__
#error " wchar_t   wide   characters   have  to be  Unicode   code   points "
#endif
#ifdef __STDC_MB_MIGHT_NEQ_WC__
#error " basic   character   codes   must   agree  on  char   and  wchar_t "
#endif

static size_t multibyte_string_length(const char src[1])
{
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    size_t len = mbsrtowcs(NULL, &src, 0, &state);

    // Restore errno on error.
    if (len == (size_t)-1)
    {
        errno = 0;
    }

    return len;
}

static void test_size_multibyte_string(const char src[static 1])
{
    const size_t n = strlen(src);
    size_t len = multibyte_string_length(src);

    printf("Size of \"%s\": %zu, %zu\n", src, n, len);

    for (size_t i = 0; i < n; ++i)
    {
        const unsigned char c = src[i];
        printf("%c(", c);

        if (c & 0x80)
        {
            printf("M");
        }

        printf("b 0x%04X),", (unsigned int)c);
    }
    printf("]\n");
}

static bool not_rel_path(const char path[static 1])
{
    return (strcmp(path, ".") != 0) && (strcmp(path, "..") != 0);
}

static int dir_selector(const struct dirent* dir)
{
    return not_rel_path(dir->d_name);
}

static void show_files(const char path[static 1])
{
    struct dirent** name_list;
#if 0
    const int scan_result
        = scandir(path, &name_list, dir_selector, alphasort);
#else
    const int scan_result
        = scandir(path, &name_list, dir_selector, alphasort);
#endif
    if (scan_result >= 0)
    {
        printf("Got %d items in %s\n", scan_result, path);
        for (int i = 0; i < scan_result; ++i)
        {
            struct dirent* dir = name_list[i];

            char full_path[4096] = "\0";
            strcpy(full_path, path);

            if ((path[strlen(path) - 1] != '/')
                && (path[strlen(path) - 1] != '\\'))
            {
                strcat(full_path, "/");
            }

            strcat(full_path, dir->d_name);

            test_size_multibyte_string(dir->d_name);

            if (dir->d_type == DT_DIR)
            {
                show_files(full_path);
            }
        }
    }
}

static void process_file_path_as_wide(const char* const src)
{
#if 1
    if (mrvn_null_or_empty(src))
    {
        return;
    }

    const size_t n = strlen(src);
    wchar_t* str_wide = malloc(n * sizeof(*str_wide));
    if (!str_wide)
    {
        return;
    }

    mbstowcs(str_wide, src, n);
#endif

    printf("Show files\n");
    show_files(src);
    // show_files(str_wide);

    free(str_wide);
}

int main(const int argc, const char* argv[static argc])
{
    //setlocale(LC_ALL, "en_US.UTF-8");
    setlocale(LC_ALL, "");

    const char* const directory_path = (argc > 1) ? argv[1] : ".";

    process_file_path_as_wide(directory_path);

    return 0;
}
