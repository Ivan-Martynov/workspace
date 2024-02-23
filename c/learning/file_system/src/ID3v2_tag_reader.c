#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "ID3v2_tag_header_reader.h"
#include "ID3v2_frame_header_reader.h"
#include "ID3v2_text_frame_reader.h"
#include "ID3v2_text_frame_ids.h"
#include "syncsafe_int_converter.h"

// static const char* const file_identifer = "ID3";

#define ID3v2_header_size 10
#define ID3v2_file_identifer_size 3

void get_tag_with_buffer(const char* const buffer)
{
    struct ID3v2_tag_header* tag_header_ptr
        = ID3v2_tag_header_parse_buffer(buffer);
    if (!tag_header_ptr || (ID3v2_tag_header_get_major_version(tag_header_ptr) != 4))
    {
        return;
    }

    char test[5];
    memcpy(test, buffer + ID3v2_header_size + 1, 4);
    test[5] = '\0';
    printf("Test = %s\n", test);
}

struct ID3v2_tag_header* get_tag_header(const char* const file_path)
{
    FILE* file_ptr = fopen(file_path, "rb");
    if (file_ptr == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", file_path);
        return NULL;
    }

    char buffer[ID3v2_header_size];
#if 1
    const size_t n = fread(buffer, sizeof(char), ID3v2_header_size, file_ptr);
#else
    fread(buffer, ID3v2_header_size, 1, file_ptr);
#endif
    fclose(file_ptr);

#if 0
    printf("Read buffer %s\n", buffer);
    for (size_t i = 0; i < ID3v2_header_size; ++i)
    {
        printf("%c ", buffer[i]);
    }
    printf("\n");
#endif

    return n < ID3v2_header_size ? NULL : ID3v2_tag_header_parse_buffer(buffer);
}

void print_tag_header_data(struct ID3v2_tag_header* const tag_header_ptr)
{
    if (!tag_header_ptr)
    {
        return;
    }

    const char major_version
        = ID3v2_tag_header_get_major_version(tag_header_ptr);
    if (major_version != 4)
    {
        printf("Invalid v2.4 tag\n");
        return;
    }

    printf("Version = %sv2.%d.%d\n", ID3v2_tag_header_get_id(tag_header_ptr),
        major_version, ID3v2_tag_header_get_revision_number(tag_header_ptr));

    // printf("Tag = %s\n", tag_header_ptr->tag);
    // printf("Version = %dx%d\n", tag_header_ptr->major_version,
    //    tag_header_ptr->minor_version);
    printf("Flags = %d\n", ID3v2_tag_header_get_flags(tag_header_ptr));
    printf("Tag size = %zu\n", ID3v2_tag_header_get_tag_size(tag_header_ptr));
}

void test_unsyncsafe(const size_t value)
{
    const size_t encoded_value = syncsafe_encode(value);
    printf("Value %zu => %zu => %zu\n", value, encoded_value,
        syncsafe_decode(encoded_value));
}

void show_mp3_tags(const char* const file_path)
{
    setlocale(LC_ALL, "");
    // test_unsyncsafe(25);
    // test_unsyncsafe(0X7F);
    // test_unsyncsafe(128);
    // test_unsyncsafe(0XFF);
    // test_unsyncsafe(0XFFFF);

    const char* dot_place = strrchr(file_path, '.');

    if (!dot_place || strcmp(++dot_place, "mp3") != 0)
    {
        // fprintf(stderr, "File %s doesn't have mp3 extension.\n", file_path);
        return;
    }

    printf("Tag data for %s\n", file_path);

    FILE* file_ptr = fopen(file_path, "rb");
    if (!file_ptr)
    {
        fprintf(stderr, "Error opening file %s\n", file_path);
        return;
    }

    struct ID3v2_tag_header* tag_header_ptr
        = ID3v2_tag_header_from_file_stream(file_ptr);
    const fpos_t tag_size
        = (fpos_t)ID3v2_tag_header_get_tag_full_size(tag_header_ptr);

    ID3v2_tag_header_print(tag_header_ptr);
    ID3v2_tag_header_delete(tag_header_ptr);

#if 1
    while (ftell(file_ptr) < tag_size)
#else
    for (fpos_t pos = ftell(file_ptr); pos < tag_size; fgetpos(file_ptr, &pos))
#endif
    {
        struct ID3v2_frame_header* frame_header_ptr
            = ID3v2_frame_header_from_file_stream(file_ptr);

        if (!frame_header_ptr)
        {
            break;
        }

        ID3v2_frame_header_print(frame_header_ptr);

        const size_t frame_size
            = ID3v2_frame_header_get_frame_size(frame_header_ptr);
        if (ID3v2_frame_header_is_text_frame(frame_header_ptr))
        {
            if (strcmp(ID3v2_frame_header_get_id(frame_header_ptr),
                    ID3v2_title_frame)
                == 0)
            {
                printf("Found title\n");
            }
            struct ID3v2_text_frame* text_frame_ptr
                = ID3v2_text_frame_from_file_stream(file_ptr, frame_size);
            ID3v2_text_frame_print(text_frame_ptr);
            ID3v2_text_frame_delete(text_frame_ptr);
        }
        else
        {
            char buffer[frame_size];
            fread(buffer, sizeof(char), frame_size, file_ptr);
        }

        ID3v2_frame_header_delete(frame_header_ptr);
    }

    fclose(file_ptr);
}
