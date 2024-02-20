#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

static const wchar_t* const file_identifer_wide = L"ID3";

#define ID3v2_header_size 10
#define ID3v2_file_identifer_size 3
//static const size_t ID3v2_file_identifer_size = 3;

struct ID3v2_header
{
    char tag[ID3v2_file_identifer_size + 1];
    char major_version;
    char minor_version;
    char flags;
    int size;
};

struct ID3v2_header_wide
{
    wchar_t tag[ID3v2_file_identifer_size];
    wchar_t major_version;
    wchar_t minor_version;
    wchar_t flags;
    int size;
};

struct ID3v2_header* ID3v2_header_new()
{
    return calloc(1, sizeof(struct ID3v2_header));
}

struct ID3v2_header_wide* ID3v2_header_wide_new()
{
    return calloc(1, sizeof(struct ID3v2_header_wide));
}

void ID3v2_delete(struct ID3v2_header* header_ptr)
{
    if (header_ptr)
    {
        free(header_ptr);
    }
}

void ID3v2_wide_delete(struct ID3v2_header_wide* header_ptr)
{
    if (header_ptr)
    {
        free(header_ptr);
    }
}

struct ID3v2_header* get_tag_header_with_buffer(char* buffer, size_t length)
{
    if (length < ID3v2_header_size)
    {
        return NULL;
    }

    if (strncmp(buffer, "ID3", ID3v2_file_identifer_size) != 0)
    {
        return NULL;
    }

    struct ID3v2_header* tag_header_ptr = ID3v2_header_new();
    if (!tag_header_ptr)
    {
        return NULL;
    }

    memset(tag_header_ptr, 0, ID3v2_header_size);
    tag_header_ptr->minor_version = 0x00;
    tag_header_ptr->major_version = 0x00;
    tag_header_ptr->flags = 0x00;
    memset(tag_header_ptr->tag, '\0', ID3v2_file_identifer_size + 1);

    int position = 0;
#if 1
    strncpy(tag_header_ptr->tag, buffer, ID3v2_file_identifer_size);
    tag_header_ptr->tag[ID3v2_file_identifer_size] = '\0';
#else
    for (size_t i = 0; i < ID3v2_file_identifer_size; ++i)
    {
        tag_header_ptr->tag[i] = buffer[i];
    }
#endif

    tag_header_ptr->major_version
        = buffer[position += ID3v2_file_identifer_size];
    tag_header_ptr->minor_version = buffer[++position];

    tag_header_ptr->flags = buffer[++position];
    //tag_header_ptr->size = syncint_decode(
    //    btoi(buffer, ID3_HEADER_SIZE, position += ID3_HEADER_FLAGS));

    return tag_header_ptr;
}

struct ID3v2_header_wide* get_tag_header_with_buffer_wide(
    wchar_t* buffer, const size_t length)
{
    if (length < ID3v2_header_size)
    {
        return NULL;
    }

    if (wcsncmp(buffer, file_identifer_wide, ID3v2_file_identifer_size) != 0)
    {
        wprintf(L"Wrong identifier: %ls, %ls\n", buffer, file_identifer_wide);
        return NULL;
    }

    struct ID3v2_header_wide* tag_header_ptr = ID3v2_header_wide_new();
    if (!tag_header_ptr)
    {
        return NULL;
    }

    memset(tag_header_ptr, 0, ID3v2_header_size);
    tag_header_ptr->minor_version = 0x00;
    tag_header_ptr->major_version = 0x00;
    tag_header_ptr->flags = 0x00;
    //memset(tag_header_ptr->tag, '\0', ID3v2_file_identifer_size);

    int position = 0;
    wcsncpy(tag_header_ptr->tag, buffer, ID3v2_file_identifer_size);
    tag_header_ptr->tag[ID3v2_file_identifer_size] = L'\0';

    tag_header_ptr->major_version
        = buffer[position += ID3v2_file_identifer_size];
    tag_header_ptr->minor_version = buffer[++position];

    tag_header_ptr->flags = buffer[++position];
    //tag_header_ptr->size = syncint_decode(
    //    btoi(buffer, ID3_HEADER_SIZE, position += ID3_HEADER_FLAGS));

    return tag_header_ptr;
}

struct ID3v2_header* get_tag_header(const char* const file_name)
{
    char buffer[ID3v2_header_size];
    FILE* file_ptr = fopen(file_name, "rb");
    if (file_ptr == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return NULL;
    }

    fread(buffer, ID3v2_header_size, 1, file_ptr);

    printf("Read buffer %s\n", buffer);
    for (size_t i = 0; i < ID3v2_header_size; ++i)
    {
        printf("%c ", buffer[i]);
    }
    printf("\n");

    fclose(file_ptr);

    return get_tag_header_with_buffer(buffer, ID3v2_header_size);
}

void print_tag_data(struct ID3v2_header* const header_ptr)
{
    printf("Tag = %s\n", header_ptr->tag);
    printf("Version = %dx%d\n", header_ptr->major_version,
        header_ptr->minor_version);
    printf("Flags = %d\n", header_ptr->flags);
}

void wprint_tag_data(struct ID3v2_header_wide* const header_ptr)
{
    wprintf(L"Tag %ls\n", header_ptr->tag);
    wprintf(L"Version %lc %lc\n", header_ptr->major_version,
        header_ptr->minor_version);
    wprintf(L"Flags %lc\n", header_ptr->flags);
}

void show_mp3_tags(const char* const file_path)
{
    char* dot_place = strrchr(file_path, '.');

    if (!dot_place || strcmp(++dot_place, "mp3") != 0)
    {
        fprintf(stderr, "File %s doesn't have mp3 extension.\n", file_path);
        return;
    }

    struct ID3v2_header* header_ptr = get_tag_header(file_path);
    print_tag_data(header_ptr);
    // printf("Test extension: %s\n", dot_place);
    ID3v2_delete(header_ptr);
}

struct ID3v2_header_wide* read_tag_header_wide(const char* const file_path)
{
    setlocale(LC_ALL, "");
    //setlocale(LC_ALL, "en_US.UTF-8");
    FILE* file_ptr = fopen(file_path, "rb, css=UTF-8");

    if (!file_ptr)
    {
        fprintf(stderr, "Error opening file %s\n", file_path);
        return NULL;
    }

    wchar_t buffer[ID3v2_header_size];
    fread(buffer, ID3v2_header_size * sizeof(wchar_t), 1, file_ptr);
    buffer[ID3v2_header_size] = L'\0';

    wprintf(L"Read buffer %ls\n", buffer);
    fclose(file_ptr);

    return get_tag_header_with_buffer_wide(buffer, ID3v2_header_size);
}

void show_mp3_tags_wide(const char* const file_path)
{
    char* dot_place = strrchr(file_path, '.');

    if (!dot_place || strcmp(++dot_place, "mp3") != 0)
    {
        fprintf(stderr, "File %s doesn't have mp3 extension.\n", file_path);
        return;
    }

    printf("Test extension: %s\n", dot_place);

    struct ID3v2_header_wide* header_ptr = read_tag_header_wide(file_path);
    wprint_tag_data(header_ptr);
    ID3v2_wide_delete(header_ptr);
#if 0

    wchar_t* dot_place = wcsrchr(file_path, L'.');

    if (!dot_place || (wcscmp(++dot_place, L"mp3") != 0))
    {
        fprintf(stderr, "File %s doesn't have mp3 extension.\n", file_path);
        return;
    }

    //struct ID3v2_header* header_ptr = get_tag_header(file_path);
    //print_tag_data(header_ptr);
    wprintf(L"Test extension: %ls\n", dot_place);
    //ID3v2_delete(header_ptr);
#endif
}
