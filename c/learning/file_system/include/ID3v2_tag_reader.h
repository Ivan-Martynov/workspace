#ifndef _H_ID3V2_TAG_READER_H_
#define _H_ID3V2_TAG_READER_H_

#endif //_H_ID3V2_TAG_READER_H_

struct ID3v2_header;

struct ID3v2_header* get_tag_header_with_buffer(char*, int);

char* ID3v2_tag_get_title(const char* const);

void show_mp3_tags(const char* const);
