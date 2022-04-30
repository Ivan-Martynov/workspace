#ifndef FILE_LINE_READER_H
#define FILE_LINE_READER_H

#include <stdio.h>
#include <wchar.h>

long int getline_wide(wchar_t** restrict line, size_t* restrict len, FILE* restrict file_p);

wchar_t* getline_from_file_wide(FILE* stream);

#endif
