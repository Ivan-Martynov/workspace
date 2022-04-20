#ifndef FILE_LINE_READER_H
#define FILE_LINE_READER_H

#include <stdio.h>
#include <wchar.h>

long int getline_from_file(char** restrict line, size_t* restrict len, FILE* restrict file_p);

long int getwline_from_file(wchar_t** restrict line, size_t* restrict len, FILE* restrict file_p);

#endif
