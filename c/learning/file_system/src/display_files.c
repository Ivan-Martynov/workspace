#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include "mrvn_string_helper.h"
#include "ID3v2_tag_reader.h"
#include "ID3v2_tag_header_reader.h"

#ifndef __STDC_ISO_10646__
#error "Wide characters have to be Unicode code points"
#endif
#ifdef __STDC_MB_MIGHT_NEQ_WC__
#error "Basic character codes must agree on char and wchar_t"
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

static int wrename_path(const wchar_t old_name_wide[static 1],
    const wchar_t new_name_wide[static 1], const bool do_rename)
{
    mbstate_t state;
    memset(&state, 0, sizeof(state));
    size_t len = wcsrtombs(NULL, &old_name_wide, 0, &state);
    if (len == (size_t)-1)
    {
        return len;
    }
    ++len;

    char prev_name[len];
    if (wcsrtombs(prev_name, &old_name_wide, len, &state) == (size_t)-1)
    {
        return -1;
    }

    memset(&state, 0, sizeof(state));
    len = wcsrtombs(NULL, &new_name_wide, 0, &state);
    if (len == (size_t)-1)
    {
        return len;
    }
    ++len;

    char next_name[len];
    if (wcsrtombs(next_name, &new_name_wide, len, &state) == (size_t)-1)
    {
        return -1;
    }

    if (do_rename)
    {
        printf("Renaming %s to %s ...", prev_name, next_name);
        if (rename(prev_name, next_name) == EXIT_SUCCESS)
        {
            printf("success.\n");
        }
        else
        {
            printf("failed: %s.\n", strerror(errno));
        }
    }
    else
    {
        //printf("Old = %s; new = %s\n", prev_name, next_name);
    }

    return 0;
}

static bool check_mp3_file_header(const char file_path[static 1])
{
    const char* const dot_place = strrchr(file_path, '.');
    if (!dot_place || (mrvn_compare_case_insensitive(dot_place + 1, "mp3") != 0))
    {
        // fprintf(stderr, "File %s doesn't have mp3 extension.\n", file_path);
        return false;
    }

    FILE* file_ptr = fopen(file_path, "rb");
    if (!file_ptr)
    {
        fprintf(stderr, "Error opening file %s\n", file_path);
        return false;
    }

    char buffer[10];
#if 1
    const size_t n = fread(buffer, sizeof(*buffer), 10, file_ptr);
#else
    const size_t n = fread(buffer, 1, sizeof(buffer), file_ptr);
#endif
    fclose(file_ptr);

    struct ID3v2_tag_header* tag_header_ptr
        = ID3v2_tag_header_parse_buffer(buffer);

    if (!tag_header_ptr)
    {
        return false;
    }

    if (n != 10)
    {
        fprintf(stderr, "Error reading file's header: %s\n", file_path);
        return false;
    }

    char header_id[4];
    memcpy(header_id, buffer, 3);
    header_id[3] = '\0';
    if (memcmp(header_id, "ID3", 3) != 0)
    {
        return false;
    }

    printf("Mp3 header: %sv2.%d.%d for %s ", header_id, buffer[3], buffer[4],
        file_path);

    return true;
}

static void test_size_multibyte_string(const char src[static 1])
{
    const size_t n = strlen(src);
    size_t len = multibyte_string_length(src);

    if (len == 0)
    {
        printf("\"%s\": size = %zu, len = %zu\n", src, n, len);
    }

    if (check_mp3_file_header(src))
    {
#if 0
        FILE* file_ptr = fopen(src, "rb");
        struct ID3v2_tag_header* tag_header_ptr
            = ID3v2_tag_header_from_file_stream(file_ptr);
        ID3v2_tag_header_print(tag_header_ptr);
        ID3v2_tag_header_delete(tag_header_ptr);
        fclose(file_ptr);
        printf("Valid ID3 tag\n");
#else
        show_mp3_tags(src);
#endif
    }
    else
    {
        //printf("Invalid ID3 v2.4 tag\n");
    }
    wrename_path(L"Один", L"Два", false);

#if 0
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
#endif
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

            test_size_multibyte_string(full_path);
            //test_size_multibyte_string(dir->d_name);

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

    //printf("Show files\n");
    show_files(src);
    // show_files(str_wide);

    free(str_wide);
}

int main(const int argc, const char* argv[static argc])
{
    setlocale(LC_ALL, "");

    const char* const directory_path = (argc > 1) ? argv[1] : ".";

    process_file_path_as_wide(directory_path);

    return 0;
}
