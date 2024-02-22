#ifndef _H_ID3V2_FRAME_HEADER_READER_H_
#define _H_ID3V2_FRAME_HEADER_READER_H_

#include <stdio.h>
#include <stddef.h>

struct ID3v2_frame_header;

struct ID3v2_frame_header* ID3v2_frame_header_init(struct ID3v2_frame_header*,
    const char* const id, const char* const flags, const size_t);

struct ID3v2_frame_header* ID3v2_frame_header_new(
    const char* const, const char* const, const size_t);

void ID3v2_frame_header_delete(struct ID3v2_frame_header*);

struct ID3v2_frame_header* ID3v2_frame_header_from_file_stream(FILE*);

void ID3v2_frame_header_print(const struct ID3v2_frame_header* const);

#endif //_H_ID3V2_FRAME_HEADER_READER_H_
