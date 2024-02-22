#include "ID3v2_tag_header_reader.h"
#include "syncsafe_int_converter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const char* const file_identifer = "ID3";

#define ID3v2_tag_header_size 10
#define ID3v2_file_identifer_size 3

struct ID3v2_tag_header
{
    char id[ID3v2_file_identifer_size + 1];
    char major_version;
    char revision_number;
    char flags;
    size_t tag_size;
    size_t extended_header_size;
};

struct ID3v2_tag_header* ID3v2_tag_header_init(
    struct ID3v2_tag_header* tag_header_ptr, const char major_version,
    const char revision_number, const char flags, const size_t tag_size,
    const size_t extended_header_size)
{
    if (tag_header_ptr)
    {
        strcpy(tag_header_ptr->id, file_identifer);

        tag_header_ptr->major_version = major_version;
        tag_header_ptr->revision_number = revision_number;
        tag_header_ptr->flags = flags;
        tag_header_ptr->tag_size = tag_size;
        tag_header_ptr->extended_header_size = extended_header_size;
    }

    return tag_header_ptr;
}

struct ID3v2_tag_header* ID3v2_tag_header_new(const char major_version,
    const char revision_number, const char flags, const size_t tag_size,
    const size_t extended_header_size)
{
    return ID3v2_tag_header_init(malloc(sizeof(struct ID3v2_tag_header)),
        major_version, revision_number, flags, tag_size, extended_header_size);
}

void ID3v2_tag_header_delete(struct ID3v2_tag_header* tag_header_ptr)
{
    if (tag_header_ptr)
    {
        free(tag_header_ptr);
    }
}

const char* ID3v2_tag_header_get_id(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->id;
}

char ID3v2_tag_header_get_major_version(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->major_version;
}

char ID3v2_tag_header_get_revision_number(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->revision_number;
}

char ID3v2_tag_header_get_flags(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->flags;
}

size_t ID3v2_tag_header_get_tag_size(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->tag_size;
}

size_t ID3v2_tag_header_get_extended_header_size(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->extended_header_size;
}

static bool is_ID3v2_header_buffer(const char* const buffer)
{
    return strncmp(buffer, file_identifer, ID3v2_file_identifer_size) == 0;
}

struct ID3v2_tag_header* ID3v2_tag_header_from_file_stream(FILE* file_ptr)
{
    char buffer[ID3v2_tag_header_size];
    const size_t n = fread(buffer, ID3v2_tag_header_size, 1, file_ptr);

    if ((n == 0) || !is_ID3v2_header_buffer(buffer))
    {
        return NULL;
    }

    return ID3v2_tag_header_parse_buffer(buffer);
}

struct ID3v2_tag_header* ID3v2_tag_header_parse_buffer(const char* const buffer)
{
    int position = ID3v2_file_identifer_size;
    const char major_version = buffer[position];

    if (major_version != 4)
    {
        return NULL;
    }
    const char revision_number = buffer[++position];
    const char flags = buffer[++position];

    printf("Major = %d; ", major_version);
    printf("revision = %d\n", revision_number);
    printf("Flags:");
    const size_t flags_int = (size_t)flags;
    for (size_t i = 0; i < 8; ++i)
    {
        printf(" %zu", ((flags_int >> i) & 1));
    }
    printf("\n");

    char tag_size_str[5];
    strncpy(tag_size_str, &buffer[++position], 4);
    tag_size_str[5] = '\0';
    const size_t tag_size = syncsafe_decode(bytes_to_size_t(tag_size_str, 4));

    printf("Size:");
    for (size_t j = 0; j < 4; ++j)
    {
        const size_t temp_int = (size_t)tag_size_str[j];
        for (size_t i = 0; i < 8; ++i)
        {
            printf(" %zu", ((temp_int >> i) & 1));
        }
    }
    printf("\n");
    printf("Size = %zu\n", tag_size);

    // Bit 6 signs if the extended header is present.
    if ((flags >> 6) & 1)
    {
        char extended_tag[5];
        strncpy(extended_tag, &buffer[position += 4], 4);
        extended_tag[5] = '\0';

        const size_t extended_header_size
            = syncsafe_decode(bytes_to_size_t(extended_tag, 4));

        return ID3v2_tag_header_new(major_version, revision_number, flags,
            tag_size, extended_header_size);
    }
    else
    {
        return ID3v2_tag_header_new(
            major_version, revision_number, flags, tag_size, 0);
    }
}