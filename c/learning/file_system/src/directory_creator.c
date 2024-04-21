#define _GNU_SOURCE

#define __STDC_WANT_LIB_EXT2__ 1

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

static void copy_file(
    const char target[restrict static 1], const char source[restrict static 1])
{
    if (!target || !source)
    {
        printf("Wrong paths\n");
        return;
    }

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
    const size_t n = 256;
    char buffer[n];
    while (true)
    {
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


static void backup_file(const char file_path[static 1])
{
    const char* const dot_place = strrchr(file_path, '/');
    if (!dot_place)
    {
        return;
    }

    const size_t n = dot_place - file_path;
    char backup_path[1024];
    strcpy(backup_path, "\0");
    //backup_path[dot_place - file_path] = '\0';
    //printf("Length = %s; %zu\n", file_path, dot_place - file_path);
    strncpy(backup_path, file_path, n);
    backup_path[n] = '\0';
    strcat(backup_path, "/backup/");
    mkdir(backup_path, 0700);
    strcat(backup_path, dot_place + 1);

    printf("Backup path = %s\n", backup_path);

    if (5 < 2)
        copy_file(backup_path, file_path);
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

    char time_buf[256];
    //strftime(time_buf, 256, "%c", gmtime(&path_stat.st_atime));
    //printf("\tAccess time: %s\n", time_buf);
    strftime(time_buf, 256, "%Y %B", gmtime(&path_stat.st_mtime));
    printf("\tModification time: %s\n", time_buf);
    //strftime(time_buf, 256, "%c", gmtime(&path_stat.st_ctime));
    //printf("\tAttribute modification time: %s\n", time_buf);
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
