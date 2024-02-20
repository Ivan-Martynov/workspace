#ifndef _H_ID3V2_TAG_READER_H_
#define _H_ID3V2_TAG_READER_H_

#endif //_H_ID3V2_TAG_READER_H_

struct ID3v2_header;
struct ID3v2_header_wide;

struct ID3v2_header* get_tag_header_with_buffer(char*, int);

void show_mp3_tags(const char* const);
void show_mp3_tags_wide(const char* const);
