#ifndef _H_ID3V2_TAG_HEADER_READER_H_
#define _H_ID3V2_TAG_HEADER_READER_H_

#include <stddef.h>
#include <stdio.h>

struct ID3v2_tag_header;

const char* ID3v2_tag_header_get_id(const struct ID3v2_tag_header* const);

char ID3v2_tag_header_get_major_version(const struct ID3v2_tag_header* const);

char ID3v2_tag_header_get_revision_number(
    const struct ID3v2_tag_header* const);

char ID3v2_tag_header_get_flags(const struct ID3v2_tag_header* const);

size_t ID3v2_tag_header_get_tag_size(const struct ID3v2_tag_header* const);

size_t ID3v2_tag_header_get_extended_header_size(
    const struct ID3v2_tag_header* const);

struct ID3v2_tag_header* ID3v2_tag_header_from_file_stream(FILE*);

struct ID3v2_tag_header* ID3v2_tag_header_parse_buffer(const char* const);

void ID3v2_tag_header_delete(struct ID3v2_tag_header*);

void ID3v2_tag_header_print(const struct ID3v2_tag_header* const);

#endif //_H_ID3V2_TAG_HEADER_READER_H_
