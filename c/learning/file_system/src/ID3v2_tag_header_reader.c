#include "ID3v2_tag_header_reader.h"
#include "syncsafe_int_converter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const char* const file_identifer = "ID3";

#define ID3V2_FILE_IDENTIFER_SIZE 3

static const size_t ID3v2_tag_header_size = 10;
static const size_t ID3v2_tag_size_size = 4;
static const size_t ID3v2_extended_tag_size = 4;

struct ID3v2_tag_header
{
    char id[ID3V2_FILE_IDENTIFER_SIZE + 1];
    char major_version;
    char revision_number;
    char flags;
    size_t tag_size;
    size_t extended_header_size;
};

struct ID3v2_tag_header* ID3v2_tag_header_init(
    struct ID3v2_tag_header* const tag_header_ptr, const char major_version,
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

void ID3v2_tag_header_print(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    if (!tag_header_ptr)
    {
        printf("Tag header is null.\n");
        return;
    }

    const char major_version
        = ID3v2_tag_header_get_major_version(tag_header_ptr);
    if (major_version != 4)
    {
        printf("Invalid v2.4 tag.\n");
        return;
    }

    printf("Version: %sv2.%d.%d\n", ID3v2_tag_header_get_id(tag_header_ptr),
        major_version, ID3v2_tag_header_get_revision_number(tag_header_ptr));
    printf("Flags: %d\n", ID3v2_tag_header_get_flags(tag_header_ptr));
    printf("Tag size: %zu\n", ID3v2_tag_header_get_tag_size(tag_header_ptr));
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

size_t ID3v2_tag_header_get_tag_full_size(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->tag_size + ID3v2_tag_header_size;
}

size_t ID3v2_tag_header_get_extended_header_size(
    const struct ID3v2_tag_header* const tag_header_ptr)
{
    return tag_header_ptr->extended_header_size;
}

static bool is_ID3v2_header_buffer(const char* const buffer)
{
    return strncmp(buffer, file_identifer, ID3V2_FILE_IDENTIFER_SIZE) == 0;
}

struct ID3v2_tag_header* ID3v2_tag_header_from_file(const char* const file_path)
{
    FILE* file_ptr = fopen(file_path, "rb");
    if (!file_ptr)
    {
        fprintf(stderr, "Error opening file %s\n", file_path);
        return NULL;
    }

    char buffer[ID3v2_tag_header_size];
    const size_t n
        = fread(buffer, sizeof(char), ID3v2_tag_header_size, file_ptr);

    fclose(file_ptr);

    if ((n < ID3v2_tag_header_size) || !is_ID3v2_header_buffer(buffer))
    {
        return NULL;
    }

    return ID3v2_tag_header_parse_buffer(buffer);
}

struct ID3v2_tag_header* ID3v2_tag_header_from_file_stream(FILE* const file_ptr)
{
#if 1
    char id_buffer[ID3V2_FILE_IDENTIFER_SIZE + 1];
    if (fread(id_buffer, sizeof(char), ID3V2_FILE_IDENTIFER_SIZE, file_ptr)
        < ID3V2_FILE_IDENTIFER_SIZE)
    {
        return NULL;
    }
    id_buffer[ID3V2_FILE_IDENTIFER_SIZE] = '\0';
    //printf("Got buffer %s\n", id_buffer);
    if (!is_ID3v2_header_buffer(id_buffer))
    {
        return NULL;
    }

    const char major_version = fgetc(file_ptr);
    const char revision_number = fgetc(file_ptr);
    const char flags = fgetc(file_ptr);

    char tag_size_str[ID3v2_tag_size_size + 1];
    if (fread(tag_size_str, sizeof(char), ID3v2_tag_size_size, file_ptr)
        < ID3v2_tag_size_size)
    {
        return NULL;
    }
    tag_size_str[ID3v2_tag_size_size] = '\0';
    const size_t tag_size
        = syncsafe_decode(bytes_to_size_t(tag_size_str, ID3v2_tag_size_size));

    // Bit 6 signs if the extended header is present.
    if ((flags >> 6) & 1)
    {
        char extended_tag[ID3v2_extended_tag_size + 1];
        if (fread(extended_tag, sizeof(char), ID3v2_extended_tag_size, file_ptr)
            < ID3v2_extended_tag_size)
        {
            return NULL;
        }
        extended_tag[ID3v2_extended_tag_size] = '\0';
        const size_t extended_header_size = syncsafe_decode(
            bytes_to_size_t(extended_tag, ID3v2_extended_tag_size));

        return ID3v2_tag_header_new(major_version, revision_number, flags,
            tag_size, extended_header_size);
    }
    else
    {
        return ID3v2_tag_header_new(
            major_version, revision_number, flags, tag_size, 0);
    }
#else
    char buffer[ID3v2_tag_header_size];
    const size_t n
        = fread(buffer, sizeof(*buffer), ID3v2_tag_header_size, file_ptr);
    //buffer[ID3v2_tag_header_size] = '\0';

    if ((n < ID3v2_tag_header_size) || !is_ID3v2_header_buffer(buffer))
    {
        return NULL;
    }

    //printf("Buffer:");
    //size_t i = 0;
    //for (; i < ID3V2_FILE_IDENTIFER_SIZE; ++i)
    //{
    //    printf(" %c", buffer[i]);
    //}
    //for (; i < ID3v2_tag_header_size; ++i)
    //{
    //    printf(" %d", buffer[i]);
    //}
    //printf("\n");

    return ID3v2_tag_header_parse_buffer(buffer);
#endif
}

struct ID3v2_tag_header* ID3v2_tag_header_parse_buffer(const char* const buffer)
{
    size_t position = ID3V2_FILE_IDENTIFER_SIZE;
    const char major_version = buffer[position++];

    if (major_version != 4)
    {
        return NULL;
    }
    const char revision_number = buffer[position++];
    const char flags = buffer[position++];

    char tag_size_str[ID3v2_tag_size_size + 1];
    memcpy(tag_size_str, &buffer[position++], ID3v2_tag_size_size);

    tag_size_str[ID3v2_tag_size_size] = '\0';
    const size_t tag_size
        = syncsafe_decode(bytes_to_size_t(tag_size_str, ID3v2_tag_size_size));

    size_t extended_header_size = 0;
    // Bit 6 signs if the extended header is present.
    if ((flags >> 6) & 1)
    {
        char extended_tag[ID3v2_extended_tag_size + 1];
        strncpy(extended_tag, &buffer[position += ID3v2_extended_tag_size],
            ID3v2_extended_tag_size);
        extended_tag[ID3v2_extended_tag_size] = '\0';

        extended_header_size = syncsafe_decode(
            bytes_to_size_t(extended_tag, ID3v2_extended_tag_size));
    }

    return ID3v2_tag_header_new(
        major_version, revision_number, flags, tag_size, extended_header_size);
}
