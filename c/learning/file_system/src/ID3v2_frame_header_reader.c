#include "ID3v2_frame_header_reader.h"
#include "syncsafe_int_converter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ID3v2_frame_header_size 10
#define ID3v2_frame_id_size 4
#define ID3v2_frame_size_size 4
#define ID3v2_frame_flags_size 2

struct ID3v2_frame_header
{
    char id[ID3v2_frame_id_size + 1];
    char flags[ID3v2_frame_flags_size + 1];
    size_t frame_size;
};

struct ID3v2_frame_header* ID3v2_frame_header_init(
    struct ID3v2_frame_header* const frame_header_ptr, const char id[static 1],
    const char flags[static 1], const size_t size)
{
    if (frame_header_ptr)
    {
        strcpy(frame_header_ptr->id, id);
        strcpy(frame_header_ptr->flags, flags);
        frame_header_ptr->frame_size = size;
    }

    return frame_header_ptr;
}

struct ID3v2_frame_header* ID3v2_frame_header_new(
    const char id[static 1], const char flags[static 1], const size_t size)
{
    return ID3v2_frame_header_init(
        malloc(sizeof(struct ID3v2_frame_header)), id, flags, size);
}

void ID3v2_frame_header_delete(struct ID3v2_frame_header* frame_header_ptr)
{
    if (frame_header_ptr)
    {
        free(frame_header_ptr);
    }
}

void ID3v2_frame_header_print(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    printf("Frame id: %s\n", frame_header_ptr->id);
    printf("Frame size: %zu\n", frame_header_ptr->frame_size);
    printf("Frame flags: %s\n", frame_header_ptr->flags);
}

const char* ID3v2_frame_header_get_id(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    return frame_header_ptr->id;
}

const char* ID3v2_frame_header_get_flags(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    return frame_header_ptr->flags;
}

size_t ID3v2_frame_header_get_frame_size(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    return frame_header_ptr->frame_size;
}

static bool is_no_more_frames(const char* const buffer)
{
    return strncmp(buffer, "\0\0\0\0", ID3v2_frame_id_size) == 0;
}

struct ID3v2_frame_header* ID3v2_frame_header_from_file(
    const char* const file_path)
{
    FILE* file_ptr = fopen(file_path, "rb");
    if (!file_ptr)
    {
        fprintf(stderr, "Error opening file %s\n", file_path);
        return NULL;
    }

    const fpos_t pos = 10;
    if (fsetpos(file_ptr, &pos) != EXIT_SUCCESS)
    {
        return NULL;
    }

    return ID3v2_frame_header_from_file_stream(file_ptr);

    char buffer[ID3v2_frame_header_size];
    const size_t n
        = fread(buffer, sizeof(char), ID3v2_frame_header_size, file_ptr);

    fclose(file_ptr);

    if ((n < ID3v2_frame_header_size) || is_no_more_frames(buffer))
    {
        return NULL;
    }

    return ID3v2_frame_header_parse_buffer(buffer);
}

struct ID3v2_frame_header* ID3v2_frame_header_parse_buffer(
    const char* const buffer)
{
    char id_buffer[ID3v2_frame_id_size + 1];
    strncpy(id_buffer, buffer, ID3v2_frame_id_size);
    id_buffer[ID3v2_frame_id_size] = '\0';

    size_t position = ID3v2_frame_id_size;

    char size_buffer[ID3v2_frame_size_size + 1];
    strncpy(size_buffer, buffer + position, ID3v2_frame_size_size);
    size_buffer[ID3v2_frame_size_size] = '\0';

    position += ID3v2_frame_size_size;

    printf("Size:");
    for (size_t i = 0; i < ID3v2_frame_size_size; ++i)
    {
        printf(" %c", size_buffer[i]);
    }

    const size_t size
        = syncsafe_decode(bytes_to_size_t(size_buffer, ID3v2_frame_size_size));
    printf("=> %zu\n", size);

    char flags_buffer[ID3v2_frame_flags_size + 1];
    strncpy(flags_buffer, buffer + position, ID3v2_frame_flags_size);
    flags_buffer[ID3v2_frame_flags_size] = '\0';

    printf("Flags:");
    for (size_t i = 0; i < ID3v2_frame_flags_size; ++i)
    {
        printf(" %c", flags_buffer[i]);
    }
    printf("\n");

    return ID3v2_frame_header_new(id_buffer, flags_buffer, size);
}

struct ID3v2_frame_header* ID3v2_frame_header_from_file_stream(
    FILE* const file_ptr)
{
    char id_buffer[ID3v2_frame_id_size + 1];
    fread(id_buffer, sizeof(char), ID3v2_frame_id_size, file_ptr);
    id_buffer[ID3v2_frame_id_size] = '\0';

    if (is_no_more_frames(id_buffer))
    {
        return NULL;
    }

    char size_buffer[ID3v2_frame_size_size + 1];
    fread(size_buffer, sizeof(char), ID3v2_frame_size_size, file_ptr);
    size_buffer[ID3v2_frame_size_size] = '\0';
    const size_t size
        = syncsafe_decode(bytes_to_size_t(size_buffer, ID3v2_frame_size_size));


    char flags_buffer[ID3v2_frame_flags_size + 1];
    fread(flags_buffer, sizeof(char), ID3v2_frame_flags_size, file_ptr);
    flags_buffer[ID3v2_frame_flags_size] = '\0';

    return ID3v2_frame_header_new(id_buffer, flags_buffer, size);
}

bool ID3v2_frame_header_is_text_frame(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    return frame_header_ptr->id[0] == 'T';
}

bool ID3v2_frame_header_is_comment_frame(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    return frame_header_ptr->id[0] == 'C';
}

bool ID3v2_frame_header_is_picture_frame(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    return frame_header_ptr->id[0] == 'A';
}
