#define _GNU_SOURCE

#define __STDC_WANT_LIB_EXT2__ 1

#include "platform_specific.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include "mrvn_string_helper.h"

#ifndef __STDC_ISO_10646__
#error "Wide characters have to be Unicode code points"
#endif
#ifdef __STDC_MB_MIGHT_NEQ_WC__
#error "Basic character codes must agree on char and wchar_t"
#endif

#if WINDOWS_PLATFORM
const char mrvn_directory_separator = '\\';
const char mrvn_directory_separator_wide = L'\\';
#else
const char mrvn_directory_separator = '/';
const char mrvn_directory_separator_wide = L'/';
#endif

static void copy_file(
    const char target[restrict static 1], const char source[restrict static 1])
{
    FILE* source_stream = fopen(source, "r");
    if (!source_stream)
    {
        printf("Couldn't open file %s for reading\n", source);
        return;
    }

    FILE* target_stream = fopen(target, "w");
    if (!target_stream)
    {
        fclose(source_stream);
        printf("Couldn't open file %s for writing\n", target);
        return;
    }

#if 0
    while (true)
    {
        const char c = fgetc(source_stream);

        if (c == EOF)
        {
            break;
        }

        fputc(c, target_stream);
    }
#else
    while (true)
    {
        const size_t n = 256;
        char buffer[n];
        const size_t bytes = fread(buffer, 1, n, source_stream);
        if (bytes == 0)
        {
            break;
        }
    //    fwrite(buffer, 1, bytes, target_stream);
    }
#endif

    fclose(source_stream);
    fclose(target_stream);
}


static void backup_file(const char file_path[static 1])
{
    const char* const dot_place = strrchr(file_path, mrvn_directory_separator);
    if (!dot_place)
    {
        return;
    }

    const size_t file_path_length = strlen(file_path);
    const size_t n = dot_place - file_path;
    const char* const backup_directory_appendix = "backup";
    const size_t shift = strlen(backup_directory_appendix) + 2;
    char* backup_path = malloc(file_path_length + shift + 1);
    if (!backup_path)
    {
        return;
    }
    strcpy(backup_path, "\0");
    //backup_path[dot_place - file_path] = '\0';
    //printf("Length = %s; %zu\n", file_path, dot_place - file_path);
    memcpy(backup_path, file_path, n);
    memcpy(backup_path + n, "/backup/", shift);
    backup_path[n + shift] = '\0';
    mkdir(backup_path, 0700);
    memcpy(backup_path + n + shift, dot_place + 1, file_path_length - n - 1);
    backup_path[n + shift + file_path_length - n - 1] = '\0';
    //strcat(backup_path, dot_place + 1);

    printf(
        "File path length = %zu; parent length = %zu, file name length = %zu\n",
        file_path_length, n, file_path_length - n);
    printf("Backup path = %s\n", backup_path);

    copy_file(backup_path, file_path);

    free(backup_path);
}

static void show_file_info(const char path[static 1])
{
    // For missing locales run $sudo locale-gen 'name_of_locale'.
    if (!setlocale(LC_TIME, "en_US.UTF-8"))
    {
        printf("Failed to set locale\n");
    }

    struct stat path_stat;
    stat(path, &path_stat);

    printf("File %s\n", path);

    const struct tm* const mod_time = gmtime(&path_stat.st_mtime);

    const size_t year_str_size = 5;
    const size_t month_str_size = 10;
    char year_str[year_str_size];
    char month_str_english[month_str_size];
    if (!strftime(year_str, year_str_size, "%Y", mod_time)
        || !strftime(month_str_english, month_str_size, "%B", mod_time))
    {
        return;
    }
    printf("\tYear/month: %s/%s\n", year_str, month_str_english);
}

static bool not_rel_path(const char path[static 1])
{
    return (strcmp(path, ".") != 0) && (strcmp(path, "..") != 0);
}

static int dir_selector(const struct dirent* dir)
{
    return not_rel_path(dir->d_name);
}

static void show_files(const char path[static 1], const bool is_recursive)
{
    struct dirent** name_list;
    const int scan_result
        = scandir(path, &name_list, dir_selector, alphasort);
    if (scan_result >= 0)
    {
        //printf("Got %d items in %s\n", scan_result, path);
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

            if (dir->d_type == DT_DIR)
            {
                if (is_recursive)
                {
                    show_files(full_path, is_recursive);
                }
            }
            else if (dir->d_type == DT_REG)
            {
                show_file_info(full_path);
                backup_file(full_path);
            }
        }
    }
}

static void process_file_path(const char* const path)
{
    if (mrvn_null_or_empty(path))
    {
        return;
    }

    const bool is_recursive = false;
    show_files(path, is_recursive);
}

int main(const int argc, const char* argv[static argc])
{
    setlocale(LC_ALL, "");

    const char* const directory_path = (argc > 1) ? argv[1] : ".";

    process_file_path(directory_path);

    return 0;
}
