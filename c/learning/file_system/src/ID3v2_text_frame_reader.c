#include "ID3v2_text_frame_reader.h"
#include "syncsafe_int_converter.h"
#include "mrvn_string_helper.h"

#include <stdlib.h>
#include <string.h>

struct ID3v2_text_frame
{
    size_t encoding;
    char* text;
};

struct ID3v2_text_frame* ID3v2_text_frame_init(
    struct ID3v2_text_frame* const text_frame_ptr, const size_t encoding,
    const char text[static 1])
{
    if (text_frame_ptr)
    {
        text_frame_ptr->encoding = encoding;

        mrvn_allocate_and_copy_char(&text_frame_ptr->text, text);
        //text_frame_ptr->text = malloc(strlen(text) * sizeof(*text));
        //strcpy(text_frame_ptr->text, text);
    }

    return text_frame_ptr;
}

struct ID3v2_text_frame* ID3v2_text_frame_new(
    const size_t encoding, const char text[static 1])
{
    return ID3v2_text_frame_init(
        malloc(sizeof(struct ID3v2_text_frame)), encoding, text);
}

void ID3v2_text_frame_delete(struct ID3v2_text_frame* text_frame_ptr)
{
    if (text_frame_ptr)
    {
        if (text_frame_ptr->text)
        {
            free(text_frame_ptr->text);
        }

        free(text_frame_ptr);
    }
}

void ID3v2_text_frame_print(const struct ID3v2_text_frame* const text_frame_ptr)
{
    if (text_frame_ptr)
    {
        printf("Encoding: %zu\n", text_frame_ptr->encoding);
        printf("Text: %s\n", text_frame_ptr->text);
    }
}

struct ID3v2_text_frame* ID3v2_text_frame_from_file_stream(
    FILE* const file_ptr, const size_t size)
{
    char encoding_str[1];
    encoding_str[0] = fgetc(file_ptr);
    encoding_str[1] = '\0';
    const size_t encoding = syncsafe_decode(bytes_to_size_t(encoding_str, 1));

    char buffer[size];
    fread(buffer, sizeof(char), size - 1, file_ptr);
    buffer[size - 1] = '\0';

    return ID3v2_text_frame_new(encoding, buffer);
}
