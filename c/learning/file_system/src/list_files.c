#define _GNU_SOURCE

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <wchar.h>
#include <locale.h>
#include "ID3v2_tag_reader.h"

const char* type_to_string(const unsigned char type)
{
    const char* result;

    switch (type)
    {
        case DT_REG:
            result = "Regular file";
            break;

        case DT_DIR:
            result = "Directory";
            break;

        case DT_FIFO:
            result = "Name pipe (FIFO)";
            break;

        case DT_SOCK:
            result = "Local-domain socket";
            break;

        case DT_CHR:
            result = "Character device";
            break;

        case DT_BLK:
            result = "Block device";
            break;

        case DT_LNK:
            result = "Symbolic link";
            break;

        case DT_UNKNOWN:
            result = "Unknown type";
            break;

        default:
            result = "Unknown type";
            break;
    }

    return result;
}

static size_t digits_in_filename(const char* const source)
{
    const size_t len = strlen(source);

    size_t i = 0;
    while ((i < len) && !isdigit((int)source[i++]))
    {
    }

    if (i == len)
    {
        return 0;
    }

    size_t digit_count = 1;
    while ((i < len) && isdigit((int)source[i++]))
    {
        ++digit_count;
    }

    return digit_count;
}

static size_t count_max_digits(struct dirent** name_list, const int item_count)
{
    size_t max_digit_count = 0;

    for (int i = 0; i < item_count; ++i)
    {
        const size_t n = digits_in_filename(name_list[i]->d_name);
        if (n > max_digit_count)
        {
            max_digit_count = n;
        }
    }

    return max_digit_count;
}

static void prepend_zero_to_number(
    const char* const source, const size_t zero_limit)
{
    const size_t len = strlen(source);

    size_t i = 0;
    while ((i < len) && !isdigit((int)source[i++]))
    {
    }

    if (i == len)
    {
        return;
    }

    size_t digit_index = i - 1;

    size_t digit_count = 1;
    while ((i < len) && isdigit((int)source[i++]))
    {
        ++digit_count;
    }

    if (digit_count >= zero_limit)
    {
        return;
    }

    char target[len + zero_limit - digit_count + 1];
    strncpy(target, source, digit_index + 1);
    target[digit_index] = '\0';

    for (size_t j = zero_limit; j > digit_count; --j)
    {
        strcat(target, "0");
    }

    // strcat(target, source + i - 2);
    strcat(target, source + digit_index);
    target[len + zero_limit] = '\0';

    printf("%s => %s\n", source, target);

#if 0
    if (rename(source, target) != -1)
    {
        printf("Renaming %s => %s\n", source, target);
    }
    else
    {
        printf("Renaming for %s failed => %s\n", source, strerror(errno));
        errno = 0;
    }
#endif

    // printf("String %s has %zu digits.\n", source, digit_count);
}

static bool not_rel_path(const char* const path)
{
    return strcmp(path, ".") && strcmp(path, "..");
}

static int dir_selector(const struct dirent* dir)
{
    return not_rel_path(dir->d_name);
    //return (strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0);
}

void list_mp3(const char* const path)
{
struct dirent** name_list;
    const int scan_result = scandir(path, &name_list, dir_selector, alphasort);

    if (scan_result >= 0)
    {
        printf("Got %d items in %s\n", scan_result, path);

        for (int i = 0; i < scan_result; ++i)
        {
            struct dirent* dir = name_list[i];
            char full_path[4096] = "\0";
            strcpy(full_path, path);

            if (path[strlen(path) - 1] != '/')
            {
                strcat(full_path, "/");
            }
            strcat(full_path, dir->d_name);

#if 0
            show_mp3_tags_wide(full_path);
#else
            show_mp3_tags(full_path);
#endif
        }
    }
}

void scan_listing(const char* const path)
{
    struct dirent** name_list;
    const int scan_result = scandir(path, &name_list, dir_selector, alphasort);

    if (scan_result >= 0)
    {
        const size_t zero_limit = count_max_digits(name_list, scan_result);
        printf("Got %d items in %s\n", scan_result, path);

        for (int i = 0; i < scan_result; ++i)
        {
            struct dirent* dir = name_list[i];
            char full_path[4096] = "\0";
            strcpy(full_path, path);

            if (path[strlen(path) - 1] != '/')
            {
                strcat(full_path, "/");
            }
            strcat(full_path, dir->d_name);

            if (dir->d_type == DT_DIR)
            {
                scan_listing(full_path);
                continue;
            }
            else
            {
                prepend_zero_to_number(full_path, zero_limit);
            }
        }
    }
}

void default_listing(const char* const path)
{
    DIR* const directory = opendir(path);
    if (directory)
    {
        struct dirent* dir;
        while ((dir = readdir(directory)))
        {
            const char* type = type_to_string(dir->d_type);

#if 1
            size_t n = strlen(type) + 1;
            // wchar_t* const w_type = malloc(sizeof(*w_type) * n);
            wchar_t w_type[n];
            mbstowcs(w_type, type, n);

            n = strlen(dir->d_name) + 1;
            //wchar_t* const w_path = malloc(sizeof(*w_path) * n);
            wchar_t w_path[n];
            mbstowcs(w_path, dir->d_name, n);

            wprintf(L"%ls: %ls\n", w_type, w_path);

            //free(w_type);
            //free(w_path);
#else
            printf("%zu => %s: %s\n", dir->d_ino, type, dir->d_name);
#endif
        }
        closedir(directory);
    }
    else
    {
        printf("Cannot open directory %s => %s\n", path, strerror(errno));
        errno = 0;
    }
}

void test_directory(const char* const directory_path)
{
    if (not_rel_path(directory_path))
    {
        //scan_listing(directory_path);
        list_mp3(directory_path);
    }
    else
    {
        // const char* test_directory_path
        //    = "C:/Users/Ivan/Downloads/books/Михаил Булгаков/";
        const char* test_directory_path
            = "C:/Users/Ivan/Downloads/books/Папа, мама, "
              "бабушка, восемь детей и грузовик/";

        list_mp3(test_directory_path);
    }
}

int main(const int argc, const char* argv[static argc])
{
    //setlocale(LC_ALL, "en_US.UTF-8");
    setlocale(LC_ALL, "");

#if 1
    const char* const directory_path = (argc > 1) ? argv[1] : ".";
    test_directory(directory_path);
#else
    const char* const directory_path = (argc > 1) ? argv[1] : ".";

    default_listing(directory_path);

    // scan_listing(directory_path);
#endif

    return 0;
}
