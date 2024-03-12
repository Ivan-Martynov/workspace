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
    return (strcmp(path, ".") != 0) && (strcmp(path, "..") != 0);
}

static int dir_selector(const struct dirent* dir)
{
    return not_rel_path(dir->d_name);
    //return (strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0);
}

static size_t number_width(size_t number)
{
    size_t width = 0;
    do
    {
        number /= 10;
        ++width;
    } while (number > 0);

    return width;
}

static void print_size(const long int size)
{
    const long int kb_threshold = 1024;
    const long int mb_threshold = kb_threshold << 10;
    const long int gb_threshold = mb_threshold << 10;
    const long int tb_threshold = gb_threshold << 10;

    if (size > tb_threshold)
    {
        printf("%.2f TB\n", (double)size / tb_threshold);
    }
    else if (size > gb_threshold)
    {
        printf("%.2f GB\n", (double)size / gb_threshold);
    }
    else if (size > mb_threshold)
    {
        printf("%.2f MB\n", (double)size / mb_threshold);
    }
    else if (size > kb_threshold)
    {
        printf("%.2f KB\n", (double)size / kb_threshold);
    }
    else
    {
        printf("%ld B\n", size);
    }
}

static void print_path_stat(const char* const path)
{
    struct stat path_stat;
    stat(path, &path_stat);

    printf("Path %s:\n", path);

    if (S_ISDIR(path_stat.st_mode))
    {
        printf("\tIs a directory\n");
    }
    else if (S_ISREG(path_stat.st_mode))
    {
        printf("\tIs a regular file\n");
    }

    printf("\tSerial number: %lu\n", path_stat.st_ino);
    printf("\tUser id of the file's owner: %u\n", path_stat.st_uid);
    printf("\tGroup id: %u\n", path_stat.st_gid);

    printf("\tSize: ");
    print_size(path_stat.st_size);
    printf("\tAmout of disk space: %ld\n", path_stat.st_blocks);
    printf("\tOptimal block size: ");
    print_size(path_stat.st_blksize);

    char time_buf[256];
    strftime(time_buf, 256, "%c", gmtime(&path_stat.st_atime));
    printf("\tAccess time: %s\n", time_buf);
    strftime(time_buf, 256, "%c", gmtime(&path_stat.st_mtime));
    printf("\tModification time: %s\n", time_buf);
    strftime(time_buf, 256, "%c", gmtime(&path_stat.st_ctime));
    printf("\tAttribute modification time: %s\n", time_buf);
}

//static void test_number_width(size_t number)
//{
//    printf("Number width test %zu => %zu\n", number, number_width(number));
//}

//[[maybe_unused]]
static void rename_mp3_by_title(const char* const path)
{
    struct dirent** name_list;
    const int scan_result = scandir(path, &name_list, dir_selector, alphasort);

#if 0
    struct stat path_stat;
    stat(path, &path_stat);

    if (S_ISDIR(path_stat.st_mode))
    {
        printf("Path %s is directory.\n", path);
    }
    else if (S_ISREG(path_stat.st_mode))
    {
        printf("Path %s is regular file.\n", path);
    }
#endif

    if (scan_result >= 0)
    {
        const size_t width = number_width(scan_result);
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

            char dir_path[4096] = "\0";
            char target_name[4096] = "\0";
            strcpy(dir_path, full_path);
            strcpy(target_name, full_path);

            strcat(full_path, dir->d_name);

            if (dir->d_type == DT_DIR)
            {
                rename_mp3_by_title(full_path);
                continue;
            }

            print_path_stat(full_path);

            const char* title = ID3v2_tag_get_title(full_path);

            char format[20];
            sprintf(format, "%%s%%0%zud %%s%%s", width);
            printf("Got format %s\n", format);

            const char* ext_pos = strrchr(dir->d_name, '.');

            char fmt[20];
            sprintf(fmt, "%%0%zud ", width);

            char prepend_number[20];
            sprintf(prepend_number, fmt, (i + 1));

            strcat(target_name, prepend_number);
            strcat(target_name, title);
            strcat(target_name, ext_pos);

#if 1
            printf("%s => %s\n", full_path, target_name);
#else
            if (rename(full_path, target_name) != -1)
            {
                printf("Renaming %s => %s\n", full_path, target_name);
            }
#endif
#if 0
            // Length of the title + the terminating characgter + prepending
            // number and one space + extension.
            char* modified_name = malloc((strlen(dir_path) + strlen(title) + 1
                                             + width + 1 + strlen(ext_pos))
                                         * sizeof(char));
            sprintf(modified_name, format, dir_path, (i + 1), title, ext_pos);

#if 1
            printf("%s => %s%s\n", full_path, dir_path, modified_name);
#else
            if (rename(full_path, modified_name) != -1)
            {
                printf("Renaming %s => %s\n", full_path, modified_name);
            }
#endif

            free(modified_name);
#endif
        }
    }
    else
    {
        printf("Failed to scan directory %s => %d\n", path, scan_result);
    }
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

            if (dir->d_type == DT_DIR)
            {
                print_path_stat(full_path);
                list_mp3(full_path);
                continue;
            }

            print_path_stat(full_path);

#if 1
            show_mp3_tags(full_path);
#else
            const char* title = ID3v2_tag_get_title(full_path);
            printf("%s => %s\n", full_path, title);
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

            prepend_zero_to_number(full_path, zero_limit);
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
        // scan_listing(directory_path);
        // rename_mp3_by_title(directory_path);
        list_mp3(directory_path);
    }
    else
    {
        // const char* test_directory_path
        //    = "C:/Users/Ivan/Downloads/books/Михаил Булгаков/";
        const char* test_directory_path
            = "C:/Users/Ivan/Downloads/books/Папа, мама, "
              "бабушка, восемь детей и грузовик/";

        rename_mp3_by_title(test_directory_path);
        // list_mp3(directory_path);
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
