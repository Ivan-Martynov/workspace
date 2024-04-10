#define _GNU_SOURCE

#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "mrvn_string_helper.h"

static bool not_rel_path_wide(const wchar_t* const path)
{
    return (wcscmp(path, L".") != 0) && (wcscmp(path, L"..") != 0);
}

static int dir_selector(const struct dirent* dir)
{
    return not_rel_path_wide(dir->d_name);
}

static void show_files_wide(const wchar_t* const path_wide)
{
    struct dirent** name_list;
    const int scan_result
        = scandir(path_wide, &name_list, dir_selector, alphasort);
}

static void process_file_path_as_wide(const char* const src)
{
    if (!mrvn_null_or_empty(src))
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

    show_files_wide(str_wide);

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
