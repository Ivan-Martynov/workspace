// mp3reader.cpp
// snags the tag off the last 128 bytes of an MP3 file.

//#include "MP3tag.h"
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>

#include <shlobj.h>
#include <wchar.h>

#define ID3_HEADER 10
#define ID3_HEADER_TAG 3
#define ID3_HEADER_VERSION 1
#define ID3_HEADER_REVISION 1
#define ID3_HEADER_FLAGS 1
#define ID3_HEADER_SIZE 4

#define NO_COMPATIBLE_TAG 0
#define ID3v23 1
#define ID3v24 2
// END TAG_HEADER CONSTANTS

/**
 * TAG_FRAME CONSTANTS
 */
#define ID3_FRAME 10
#define ID3_FRAME_ID 4
#define ID3_FRAME_SIZE 4
#define ID3_FRAME_FLAGS 2
#define ID3_FRAME_ENCODING 1
#define ID3_FRAME_LANGUAGE 3
#define ID3_FRAME_SHORT_DESCRIPTION 1

#define INVALID_FRAME 0
#define TEXT_FRAME 1
#define COMMENT_FRAME 2
#define APIC_FRAME 3

#define ISO_ENCODING 0
#define UTF_16_ENCODING 1
// END TAG_FRAME CONSTANTS

/**
 * FRAME IDs
 */
#define TITLE_FRAME_ID "TIT2"
#define ARTIST_FRAME_ID "TPE1"
#define ALBUM_FRAME_ID "TALB"
#define ALBUM_ARTIST_FRAME_ID "TPE2"
#define GENRE_FRAME_ID "TCON"
#define TRACK_FRAME_ID "TRCK"
#define YEAR_FRAME_ID "TYER"
#define COMMENT_FRAME_ID "COMM"
#define DISC_NUMBER_FRAME_ID "TPOS"
#define COMPOSER_FRAME_ID "TCOM"
#define ALBUM_COVER_FRAME_ID "APIC"
// END FRAME IDs

/**
 * APIC FRAME CONSTANTS
 */
#define ID3_FRAME_PICTURE_TYPE 1
#define JPG_MIME_TYPE "image/jpeg"
#define PNG_MIME_TYPE "image/png"

// Picture types:
#define OTHER 0x00
#define FILE_ICON 0x01
#define OTHER_FILE_ICON 0x02
#define FRONT_COVER 0x03
#define BACK_COVER 0x04
#define LEAFLET_PAGE 0x05
#define MEDIA 0x06
#define LEAD_ARTIST 0x07
#define ARTIST 0x08
#define CONDUCTOR 0x09
#define BAND 0x0A
#define COMPOSER 0x0B
#define LYRICIST 0x0C
#define RECORDING_LOCATION 0x0D
#define DURING_RECORDING 0x0E
#define DURING_PERFORMANCE 0x0F
#define VIDEO_SCREEN_CAPTURE 0x10
#define A_BRIGHT_COLOURED_FISH 0x11
#define ILLUSTRATION 0x12
#define ARTIST_LOGOTYPE 0x13
#define PUBLISHER_LOGOTYPE 0x14

typedef struct
{
    char tag[ID3_HEADER_TAG];
    char major_version;
    char revision_number;
    char flags;
    int tag_size;
} ID3v2_header;

typedef struct
{
    int size;
    char encoding;
    char* data;
} ID3v2_frame_text_content;

typedef struct
{
    char* language;
    char* short_description;
    ID3v2_frame_text_content* text;
} ID3v2_frame_comment_content;

typedef struct
{
    char encoding;
    char* mime_type;
    char picture_type;
    char* description;
    int picture_size;
    char* data;
} ID3v2_frame_apic_content;

typedef struct
{
    char frame_id[ID3_FRAME_ID];
    int size;
    char flags[ID3_FRAME_FLAGS];
    char* data;
} ID3v2_frame;

typedef struct _ID3v2_frame_list
{
    ID3v2_frame* frame;
    struct _ID3v2_frame_list* start;
    struct _ID3v2_frame_list* last;
    struct _ID3v2_frame_list* next;
} ID3v2_frame_list;

typedef struct
{
    char* raw;
    ID3v2_header* tag_header;
    ID3v2_frame_list* frames;
} ID3v2_tag;

// Constructor functions
ID3v2_header* new_header();
ID3v2_tag* new_tag();
ID3v2_frame* new_frame();
ID3v2_frame_list* new_frame_list();
ID3v2_frame_text_content* new_text_content(int size);
ID3v2_frame_comment_content* new_comment_content(int size);
ID3v2_frame_apic_content* new_apic_content();

ID3v2_tag* new_tag()
{
    ID3v2_tag* tag = (ID3v2_tag*)malloc(sizeof(ID3v2_tag));
    tag->tag_header = new_header();
    tag->frames = new_frame_list();
    return tag;
}

ID3v2_header* new_header()
{
    ID3v2_header* tag_header = (ID3v2_header*)malloc(sizeof(ID3v2_header));
    if (tag_header != NULL)
    {
        memset(tag_header->tag, '\0', ID3_HEADER_TAG);
        tag_header->revision_number = 0x00;
        tag_header->major_version = 0x00;
        tag_header->flags = 0x00;
        memset(tag_header->tag, 0, ID3_HEADER_SIZE);
    }
    return tag_header;
}

ID3v2_frame* new_frame()
{
    ID3v2_frame* frame = (ID3v2_frame*)malloc(sizeof(ID3v2_frame));
    return frame;
}

ID3v2_frame_list* new_frame_list()
{
    ID3v2_frame_list* list
        = (ID3v2_frame_list*)malloc(sizeof(ID3v2_frame_list));
    if (list != NULL)
    {
        list->frame = NULL;
        list->next = NULL;
        list->start = NULL;
    }
    return list;
}

ID3v2_frame_text_content* new_text_content(int size)
{
    ID3v2_frame_text_content* content
        = (ID3v2_frame_text_content*)malloc(sizeof(ID3v2_frame_text_content));
    content->data = (char*)malloc(size * sizeof(char));
    return content;
}

ID3v2_frame_comment_content* new_comment_content(int size)
{
    ID3v2_frame_comment_content* content = (ID3v2_frame_comment_content*)malloc(
        sizeof(ID3v2_frame_comment_content));
    content->text = new_text_content(
        size - ID3_FRAME_SHORT_DESCRIPTION - ID3_FRAME_LANGUAGE);
    content->language = (char*)malloc(ID3_FRAME_LANGUAGE + sizeof(char));
    return content;
}

ID3v2_frame_apic_content* new_apic_content()
{
    ID3v2_frame_apic_content* content
        = (ID3v2_frame_apic_content*)malloc(sizeof(ID3v2_frame_apic_content));
    return content;
}

int syncint_encode(int value)
{
    int out, mask = 0x7F;

    while (mask ^ 0x7FFFFFFF)
    {
        out = value & ~mask;
        out <<= 1;
        out |= value & mask;
        mask = ((mask + 1) << 8) - 1;
        value = out;
    }

    return out;
}

int syncint_decode(int value)
{
    unsigned int a, b, c, d, result = 0x0;
    a = value & 0xFF;
    b = (value >> 8) & 0xFF;
    c = (value >> 16) & 0xFF;
    d = (value >> 24) & 0xFF;

    result = result | a;
    result = result | (b << 7);
    result = result | (c << 14);
    result = result | (d << 21);

    return result;
}

unsigned int btoi(char* bytes, int size, int offset)
{
    unsigned int result = 0x00;
    int i = 0;
    for (i = 0; i < size; i++)
    {
        result = result << 8;
        result = result | (unsigned char)bytes[offset + i];
    }

    return result;
}

char* itob(int integer)
{
    int size = 4;
    char* result = (char*)malloc(sizeof(char) * size);

    // We need to reverse the bytes because Intel uses little endian.
    char* aux = (char*)&integer;
    for (int i = size - 1; i >= 0; i--)
    {
        result[size - 1 - i] = aux[i];
    }

    return result;
}

int has_id3v2tag(ID3v2_header* tag_header)
{
    if (memcmp(tag_header->tag, "ID3", 3) == 0)
    {
        return 1;
    }

    return 0;
}

int get_tag_version(ID3v2_header* tag_header)
{
    if (tag_header->major_version == 3)
    {
        return ID3v23;
    }
    else if (tag_header->major_version == 4)
    {
        return ID3v24;
    }
    else
    {
        return NO_COMPATIBLE_TAG;
    }
}

//---------------------------------- FRAME -----------------------------------
ID3v2_frame* parse_frame(char* bytes, int offset, int version)
{
    ID3v2_frame* frame = new_frame();

    // Parse frame header
    memcpy(frame->frame_id, bytes + offset, ID3_FRAME_ID);
    // Check if we are into padding
    if (memcmp(frame->frame_id, "\0\0\0\0", 4) == 0)
    {
        return NULL;
    }

    frame->frame_id[4] = '\0';
    frame->size = btoi(bytes, 4, offset += ID3_FRAME_ID);
    if (version == ID3v24)
    {
        frame->size = syncint_decode(frame->size);
    }
    // printf("SIZE:%d\n",frame->size);
    if (frame->size <= 0)
        return NULL;

    memcpy(frame->flags, bytes + (offset += ID3_FRAME_SIZE), 2);

    // Load frame data
    frame->data = (char*)malloc(frame->size * sizeof(char*));
    // printf("\nparse_frame-2\n");
    memcpy(frame->data, bytes + (offset += ID3_FRAME_FLAGS), frame->size);

    return frame;
}

int get_frame_type(char* frame_id)
{
    switch (frame_id[0])
    {
        case 'T':
            return TEXT_FRAME;
        case 'C':
            return COMMENT_FRAME;
        case 'A':
            return APIC_FRAME;
        default:
            return INVALID_FRAME;
    }
}

void add_to_list(ID3v2_frame_list* main, ID3v2_frame* frame)
{
    // if empty list
    if (main->start == NULL)
    {
        main->start = main;
        main->last = main;
        main->frame = frame;
    }
    else
    {
        ID3v2_frame_list* current = new_frame_list();
        current->frame = frame;
        current->start = main->start;
        main->last->next = current;
        main->last = current;
    }
}

int _has_id3v2tag(char* raw_header)
{
    if (memcmp(raw_header, "ID3", 3) == 0)
    {
        return 1;
    }

    return 0;
}

ID3v2_header* get_tag_header_with_buffer(char* buffer, int length)
{
    if (length < ID3_HEADER)
    {
        return NULL;
    }
    if (!_has_id3v2tag(buffer))
    {
        return NULL;
    }
    ID3v2_header* tag_header = new_header();

    int position = 0;
    memcpy(tag_header->tag, buffer, ID3_HEADER_TAG);
    tag_header->major_version = buffer[position += ID3_HEADER_TAG];
    tag_header->revision_number = buffer[position += ID3_HEADER_VERSION];
    tag_header->flags = buffer[position += ID3_HEADER_REVISION];
    tag_header->tag_size = syncint_decode(
        btoi(buffer, ID3_HEADER_SIZE, position += ID3_HEADER_FLAGS));

    return tag_header;
}

ID3v2_tag* load_tag_with_buffer(char* bytes, int length)
{
    int i = 0, picDes = 0;
    WCHAR* wide_buff_ptr;
    FILE* pFile;

    // Declaration
    ID3v2_tag* tag;
    ID3v2_header* tag_header;

    // Initialization
    tag_header = get_tag_header_with_buffer(bytes, length);

    if (tag_header == NULL || get_tag_version(tag_header) == NO_COMPATIBLE_TAG)
    {
        // No compatible ID3 tag in the file, or we got some problem opening the
        // file
        return NULL;
    }

    tag = new_tag();
    if (tag->tag_header->tag_size < length)
    {
        // Not enough bytes provided to parse completely. TODO: how to
        // communicate to the user the lack of bytes?
        return NULL;
    }

    // Associations
    tag->tag_header = tag_header;

    tag->raw = (char*)malloc(tag->tag_header->tag_size * sizeof(char));
    // memcpy(tag->raw, bytes, length); //----------------------------------> IT
    // WAS AN ERROR
    memcpy(tag->raw, &bytes[10], length);

    // printf("\n==>%d\n",tag->tag_header->tag_size);

    int offset = 0;
    while (offset < tag->tag_header->tag_size)
    {
        ID3v2_frame* frame = {0};
        frame = parse_frame(tag->raw, offset, get_tag_version(tag_header));

        if (frame != NULL)
        {
            // offset += frame->size + 10;
            add_to_list(tag->frames, frame);

            //------------------------------------------
            // printf("\n\nOffset: %d, ",offset);
            // printf("Frame Size: %d",frame->size);
            // printf("Frame Flags: %s\n",frame->flags);
            printf("%0.4s: ", frame->frame_id);
            // printf("Frame Data: %s\n",frame->data);

            if (memcmp(frame->frame_id, "APIC", 4) != 0)
            {
                for (i = 0; i < frame->size; i++)
                {
                    if (frame->data[i] >= 32)
                        printf("%c", frame->data[i]);
                }
            }
            else
            {
                //----------- Reading album art here into jpg file -----------
                pFile = fopen("art.jpg", "wb");
                fwrite(&bytes[offset + 12 + 22], sizeof(char), frame->size - 14,
                    pFile);
                fclose(pFile);
                //----------- Reading album art here into jpg file -----------

                printf("%d bytes", frame->size - 14); // Mind 14 bytes here
            }
            printf("\n");

            offset += frame->size + 10;
            //------------------------------------------
        }
        else
        {
            printf("\nALL FRAMES ARE PARSED.....\n");
            break;
        }
    }
    return tag;
}

ID3v2_header* get_tag_header(const char* file_name)
{
    char buffer[ID3_HEADER];
    FILE* file = fopen(file_name, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    fread(buffer, ID3_HEADER, 1, file);
    fclose(file);
    return get_tag_header_with_buffer(buffer, ID3_HEADER);
}

ID3v2_tag* load_tag(const char* file_name)
{
    // get header size
    ID3v2_header* tag_header = get_tag_header(file_name);
    if (tag_header == NULL)
    {
        return NULL;
    }
    int header_size = tag_header->tag_size;

    //--------------------------------------------------
    // printf("Major:%d\n",tag_header->major_version);
    // printf("Minor:%d\n",tag_header->minor_version);
    // printf("Flags:%c\n",tag_header->flags);
    // printf("TAG_SIZE:%d\n",tag_header->tag_size);
    //--------------------------------------------------
    printf("\n");

    free(tag_header);

    // allocate buffer and fetch header
    FILE* file;
    file = fopen(file_name, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }
    char* buffer = (char*)malloc((10 + header_size) * sizeof(char));
    if (buffer == NULL)
    {
        perror("Could not allocate buffer");
        fclose(file);
        return NULL;
    }
    // fseek(file, 10, SEEK_SET);
    fread(buffer, header_size + 10, 1, file);
    fclose(file);

    // parse free and return
    ID3v2_tag* tag = load_tag_with_buffer(buffer, header_size);
    free(buffer);

    return tag;
}

int main(int argc, char* argv[])
{
    ID3v2_tag* ptr = load_tag(argv[1]);

    printf("\nCame here...\n");
    // getchar();
}
