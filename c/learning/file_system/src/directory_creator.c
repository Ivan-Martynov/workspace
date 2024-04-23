#define _GNU_SOURCE

#define __STDC_WANT_LIB_EXT2__ 1

#include "platform_specific.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include "mrvn_string_helper.h"
#include "mrvn_type_constants.h"

#ifndef __STDC_ISO_10646__
#error "Wide characters have to be Unicode code points"
#endif
#ifdef __STDC_MB_MIGHT_NEQ_WC__
#error "Basic character codes must agree on char and wchar_t"
#endif

#if WINDOWS_PLATFORM
const char mrvn_directory_separator = '/';
const char mrvn_directory_separator_wide = L'/';
#else
const char mrvn_directory_separator = '/';
const char mrvn_directory_separator_wide = L'/';
#endif

static void copy_file(
    const char target[restrict static 1], const char source[restrict static 1])
{
    return; // Disable for not overwriting.

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
        fwrite(buffer, 1, bytes, target_stream);
    }
#endif

    fclose(source_stream);
    fclose(target_stream);
}

static size_t mrvn_get_parent_directory(
    char target[restrict static 1], const char file_path[restrict static 1])
{
    const char* const dot_place = strrchr(file_path, mrvn_directory_separator);
    if (!dot_place)
    {
        return 0;
    }

    const size_t parent_dir_length = dot_place + 1 - file_path;
    memcpy(target, file_path, parent_dir_length);
    target[parent_dir_length] = '\0';
    return parent_dir_length;
}

static void backup_file(const char file_path[static 1], const bool do_backup)
{
    if (!do_backup)
    {
        return;
    }
    const char* const dot_place = strrchr(file_path, mrvn_directory_separator);
    if (!dot_place)
    {
        return;
    }

    const size_t file_path_length = mrvn_multibyte_string_length(file_path);
    size_t n = dot_place - file_path;
    const char* const backup_directory_appendix = "backup";
    const size_t shift = strlen(backup_directory_appendix) + 1;
    char* backup_path = malloc(file_path_length + shift + 1);
    if (!backup_path)
    {
        return;
    }
    //strcpy(backup_path, "\0");
    //backup_path[dot_place - file_path] = '\0';
    //printf("Length = %s; %zu\n", file_path, dot_place - file_path);
    //memcpy(backup_path, file_path, n);
    mrvn_get_parent_directory(backup_path, file_path);
    memcpy(backup_path + n, "backup/", shift);
    backup_path[n + shift] = '\0';
    if (mkdir(backup_path, 0700) != EXIT_SUCCESS)
    {
        printf("Cannot create folder: %s\n", strerror(errno));
    }
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

static void move_file_to_year_month_folder(
    const char file_path[static 1], const bool do_move)
{
    char* current_locale = setlocale(LC_TIME, mrvn_null_ptr);
    char* locale_backup = mrvn_allocate_char_from_another(current_locale);
    if (!locale_backup)
    {
        return;
    }

    // For missing locales run $sudo locale-gen 'name_of_locale'.
    if (!setlocale(LC_TIME, "en_US.UTF-8") && !setlocale(LC_TIME, ""))
    {
        printf("Failed to set locale\n");
        setlocale(LC_TIME, locale_backup);
        free(locale_backup);
        return;
    }

    struct stat path_stat;
    stat(file_path, &path_stat);

    printf("File %s\n", file_path);

    const struct tm* const mod_time = gmtime(&path_stat.st_mtime);

    printf("Length of year output %zu\n", strftime(NULL, 100, "/%Y/", mod_time));

    size_t year_str_size = 32;
    char year_str[year_str_size];
    year_str_size = strftime(year_str, year_str_size, "%Y/", mod_time);

    size_t month_str_size = 128;
    char month_str_english[month_str_size];
    month_str_size
        = strftime(month_str_english, month_str_size, "%B/", mod_time);

    // Will not work with multibyte string.
    month_str_english[0] = toupper(month_str_english[0]);

    setlocale(LC_TIME, locale_backup);
    free(locale_backup);

    if ((year_str_size == 0) || (month_str_size == 0))
    {
        return;
    }
    printf("\tYear/month: %s%s\n", year_str, month_str_english);

    const size_t file_path_length = strlen(file_path);
    char* target_path
        = malloc(file_path_length + year_str_size + month_str_size + 1);
    if (!target_path)
    {
        return;
    }

    const size_t n = mrvn_get_parent_directory(target_path, file_path);
    if (n == 0)
    {
        goto cleanup_label;
    }

    printf("Current = %s\n", target_path);

    // Year string already contains the terminating character
    memcpy(target_path + n, year_str, year_str_size + 1);
    //target_directory[n + year_str_size - 1] = '\0';
    if (mkdir(target_path, 0700) != EXIT_SUCCESS)
    {
        printf("Cannot create folder: %s\n", strerror(errno));
        errno = 0;
    }

    printf("With year = %s\n", target_path);

    memcpy(target_path + n + year_str_size, month_str_english,
        month_str_size + 1);

    if (mkdir(target_path, 0700) != EXIT_SUCCESS)
    {
        printf("Cannot create folder: %s\n", strerror(errno));
        errno = 0;
    }
    printf("With month = %s\n", target_path);

    // strcat(target_directory, file_path + n + 1);
    memcpy(target_path + n + year_str_size + month_str_size, file_path + n,
        file_path_length - n);
    target_path[year_str_size + month_str_size + file_path_length] = '\0';
    if (do_move)
    {
        if (rename(file_path, target_path) != EXIT_SUCCESS)
        {
            printf("Cannot copy file: %s\n", strerror(errno));
            errno = 0;
        }
    }
    else
    {
        printf("Final path = %s; %zu vs %zu\n", target_path,
            year_str_size + month_str_size + file_path_length,
            strlen(target_path));
    }
cleanup_label:
    free(target_path);
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
                move_file_to_year_month_folder(full_path, false);
                backup_file(full_path, false);
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
