#include "ID3v2_frame_header_reader.h"
#include "syncsafe_int_converter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    struct ID3v2_frame_header* frame_header_ptr, const char* const id,
    const char* const flags, const size_t size)
{
    if (frame_header_ptr)
    {
        printf("Copying buffer: %s %zu\n", id, strlen(id));
        //strcpy(frame_header_ptr->id, id);
        strncpy(frame_header_ptr->id, id, ID3v2_frame_id_size);
        frame_header_ptr->id[ID3v2_frame_id_size + 1] = '\0';

        strncpy(frame_header_ptr->flags, flags, ID3v2_frame_flags_size);
        frame_header_ptr->flags[ID3v2_frame_flags_size + 1] = '\0';
        frame_header_ptr->frame_size = size;
    }

    return frame_header_ptr;
}

struct ID3v2_frame_header* ID3v2_frame_header_new(
    const char* const id, const char* const flags, const size_t size)
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

//static bool is_no_more_frames(const char* const buffer)
//{
//    return strncmp(buffer, "\0\0\0\0", ID3v2_frame_id_size) == 0;
//}

struct ID3v2_frame_header* ID3v2_frame_header_from_file_stream(FILE* file_ptr)
{
    char id_buffer[ID3v2_frame_id_size + 1];
    for (size_t i = 0; i < ID3v2_frame_id_size; ++i)
    {
        const unsigned char c = fgetc(file_ptr);
        printf("%c", c);
        id_buffer[i] = c;
    }
    printf("\n");

    // const size_t n
    //    = fread(id_buffer, sizeof(char), ID3v2_frame_id_size, file_ptr);
    id_buffer[ID3v2_frame_id_size + 1] = '\0';
    printf("got buffer 0:%s\n", id_buffer);

    //if (is_no_more_frames(id_buffer))
    //{
    //    return NULL;
    //}

    //char id[ID3v2_frame_id_size + 1];
    for (size_t i = 0; i < ID3v2_frame_id_size; ++i)
    {
        printf("%c", id_buffer[i]);
        //id[i] = id_buffer[i];
    }
    printf("\n");

    // strncpy(id, id_buffer, ID3v2_frame_id_size);
    //id[ID3v2_frame_id_size + 1] = '\0';
    printf("got buffer:%s\n", id_buffer);
    //printf("got id: %s\n", id);

    char size_buffer[ID3v2_frame_size_size + 1];
    fread(size_buffer, ID3v2_frame_size_size, 1, file_ptr);
    size_buffer[ID3v2_frame_size_size + 1] = '\0';
    const size_t size
        = syncsafe_decode(bytes_to_size_t(size_buffer, ID3v2_frame_size_size));
    printf("Size = %zu\n", size);

    char flags_buffer[ID3v2_frame_flags_size + 1];
    fread(flags_buffer, ID3v2_frame_flags_size, 1, file_ptr);
    flags_buffer[ID3v2_frame_flags_size + 1] = '\0';

    return ID3v2_frame_header_new(id_buffer, flags_buffer, size);
    // return ID3v2_frame_header_new(id, flags_buffer, size);
}

void ID3v2_frame_header_print(
    const struct ID3v2_frame_header* const frame_header_ptr)
{
    printf("Frame id %s\n", frame_header_ptr->id);
    printf("Frame size %zu\n", frame_header_ptr->frame_size);
    printf("Frame flags %s\n", frame_header_ptr->flags);

}
