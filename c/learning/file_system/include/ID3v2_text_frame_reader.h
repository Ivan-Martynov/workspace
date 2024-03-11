#ifndef _H_ID3V2_TEXT_FRAME_READER_H_
#define _H_ID3V2_TEXT_FRAME_READER_H_

#include <stddef.h>
#include <stdio.h>

struct ID3v2_text_frame;

struct ID3v2_text_frame* ID3v2_text_frame_init(
    struct ID3v2_text_frame* const, const size_t, const char[static 1]);

struct ID3v2_text_frame* ID3v2_text_frame_new(
    const size_t, const char[static 1]);

void ID3v2_text_frame_print(const struct ID3v2_text_frame* const);

void ID3v2_text_frame_delete(struct ID3v2_text_frame*);

const char* ID3v2_text_frame_get_text(const struct ID3v2_text_frame* const);

struct ID3v2_text_frame* ID3v2_text_frame_from_file_stream(
    FILE* const, const size_t);

#endif //_H_ID3V2_TEXT_FRAME_READER_H_
