#include "ID3v2_text_frame_ids.h"
#include "mrvn_map_char.h"

void ID3v2_frame_ids_fill_map(struct mrvn_map_char* const map_ptr)
{
    const struct
    {
        const char* key;
        char* value;
    } temp_array[] = {
        {ID3v2_content_group_frame, "Content group frame"},
        {ID3v2_title_frame, "Title"},
        {ID3v2_album_frame, "Album"},
        {ID3v2_original_album_frame, "Original album"},
        {ID3v2_track_number_frame, "Track number"},
        {ID3v2_part_of_set_frame, "Part of set"},
        {ID3v2_subtitle_frame, "Set subtitle"},
        {ID3v2_ISRC_frame, "ISRC"},
    };

    const size_t n = sizeof(temp_array) / sizeof(*temp_array);
    for (size_t i = 0; i < n; ++i)
    {
        mrvn_map_char_insert(map_ptr, temp_array[i].key, temp_array[i].value);
    }
}

const char* ID3v2_frame_get_title(struct mrvn_map_char* const map_ptr)
{
    return mrvn_map_char_get_value(map_ptr, ID3v2_title_frame);
}
