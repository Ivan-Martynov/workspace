#ifndef _H_ID3V2_FRAME_HEADER_READER_H_
#define _H_ID3V2_FRAME_HEADER_READER_H_

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

struct ID3v2_frame_header;

struct ID3v2_frame_header* ID3v2_frame_header_init(
    struct ID3v2_frame_header* const, const char id[static 1],
    const char flags[static 1], const size_t);

struct ID3v2_frame_header* ID3v2_frame_header_new(
    const char[static 1], const char[static 1], const size_t);

void ID3v2_frame_header_delete(struct ID3v2_frame_header*);

void ID3v2_frame_header_print(const struct ID3v2_frame_header* const);

const char* ID3v2_frame_header_get_id(const struct ID3v2_frame_header* const);

const char* ID3v2_frame_header_get_flags(
    const struct ID3v2_frame_header* const);

size_t ID3v2_frame_header_get_frame_size(
    const struct ID3v2_frame_header* const);

struct ID3v2_frame_header* ID3v2_frame_header_from_file(const char* const);

struct ID3v2_frame_header* ID3v2_frame_header_parse_buffer(const char* const);

struct ID3v2_frame_header* ID3v2_frame_header_from_file_stream(FILE* const);

bool ID3v2_frame_header_is_text_frame(const struct ID3v2_frame_header* const);

bool ID3v2_frame_header_is_comment_frame(
    const struct ID3v2_frame_header* const);

bool ID3v2_frame_header_is_picture_frame(
    const struct ID3v2_frame_header* const);

#endif //_H_ID3V2_FRAME_HEADER_READER_H_
