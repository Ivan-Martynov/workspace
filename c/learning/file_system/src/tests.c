#include "mrvn_map_char.h"
#include "mrvn_string_helper.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

static bool create_file(const char* const file_path, const char* const content)
{
    FILE* stream = fopen(file_path, "w");
    if (stream)
    {
        fprintf(stream, "File named with %s.\n", file_path);
        if (!mrvn_null_or_empty(content))
        {
            fprintf(stream, "%s\n", content);
        }

        fclose(stream);

        return true;
    }
    else
    {
        printf("Failed to create file %s\n", file_path);
        return false;
    }
}

static void create_filenames_with_end_of_line(void)
{
    create_file("\n",
        "Creating a file with file name as the end of line is likely to "
        "have a weird symbol instead of the end of line symbol.");
    create_file("\nFile_name_with_end_of_line_at_the_beginning_of_it",
        "Creating a file with file name starting with the end of line is "
        "likely to "
        "have a weird symbol instead of the end of line symbol.");
    create_file("File_name_with_end_of_line_at_the_end_of_it\n",
        "Creating a file with file name ending with the end of line is likely "
        "to "
        "have a weird symbol instead of the end of line symbol.");
    create_file("File_name_with_end_of_line\nin_the_middle_of_it",
        "Creating a file with file name having the end of line is likely to "
        "have a weird symbol instead of the end of line symbol.");
}

void create_dangerous_filenames(void)
{
    return;
    create_file(
        "-n", "Might have problems with terminal commands having options.");

    create_filenames_with_end_of_line();
}

void print_map_char(const struct mrvn_map_char* const map_ptr)
{
    const size_t map_size = mrvn_map_char_size(map_ptr);
    if (map_size > 0)
    {
        printf("Printing map (%zu elements):\n", map_size);
    }
    else
    {
        printf("Map is empty.\n");
    }

    for (size_t i = 0; i < map_size; ++i)
    {
        printf("%zu: [%s] = %s\n", i, mrvn_map_char_get_key_at(map_ptr, i),
            mrvn_map_char_get_value_at(map_ptr, i));
        // printf("Key/value at %zu = %s/%s\n", i,
        //    mrvn_map_char_get_key_at(map_ptr, i),
        //    mrvn_map_char_get_value_at(map_ptr, i));
    }
}

void test_1(void)
{
    const char* keys[] = {"B", "D", "F", "K", "M", "R"};
    const char* values[]
        = {"Value B", "Value D", "Value F", "Value K", "Value M", "Value R"};

    const size_t keys_size = sizeof(keys) / sizeof(*keys);
    const size_t values_size = sizeof(values) / sizeof(*values);
    const size_t size = (keys_size >= values_size) ? values_size : keys_size;

    const size_t capacity = 0;
    struct mrvn_map_char* map_ptr = mrvn_map_char_new(capacity, 0);

    assert(mrvn_map_char_size(map_ptr) == 0);
    assert(mrvn_map_char_capacity(map_ptr) == capacity);

    if (!map_ptr)
    {
        printf("Failed to create a map.\n");
        return;
    }

    printf("Created map with capacity %zu and size %zu\n",
        mrvn_map_char_capacity(map_ptr), mrvn_map_char_size(map_ptr));

    print_map_char(map_ptr);

    for (size_t i = 0; i < size; ++i)
    {
        mrvn_map_char_insert(map_ptr, keys[i], values[i]);
    }

    assert(mrvn_map_char_size(map_ptr) == size);

    mrvn_map_char_insert(map_ptr, "E", "Value E");
    mrvn_map_char_insert(map_ptr, "A", "Value A");
    mrvn_map_char_insert(map_ptr, "S", "Value S");
    mrvn_map_char_insert(map_ptr, "R", "New value R");

    print_map_char(map_ptr);

    mrvn_map_char_remove_at(map_ptr, 4);
    mrvn_map_char_remove(map_ptr, "S");

    print_map_char(map_ptr);

    assert(!mrvn_map_char_get_value(map_ptr, "T"));
    assert(strcmp(mrvn_map_char_get_value(map_ptr, "A"), "Value A") == 0);

    printf("Removing elements.\n");
    for (int i = mrvn_map_char_size(map_ptr) - 1; i >= 0; --i)
    {
        printf("Removing at %d of %zu.\n", i, mrvn_map_char_size(map_ptr));
        mrvn_map_char_remove_at(map_ptr, i);
    }

    print_map_char(map_ptr);

    mrvn_map_char_delete(map_ptr);
}

static void compare_two_strings(
    const char first[restrict static 1], const char second[restrict static 1])
{
    if (strcmp(first, second) == 0)
    {
        printf(
            "Strings \"%s\" and \"%s\" are strictly equal.\n", first, second);
    }
    else
    {
        printf("Strings \"%s\" and \"%s\" are not strictly equal.\n", first,
            second);
    }

    if (mrvn_equals_case_insensitive(first, second))
    {
        printf("Strings \"%s\" and \"%s\" are equal with case ignored.\n",
            first, second);
    }
    else
    {
        printf("Strings \"%s\" and \"%s\" are not equal.\n", first, second);
    }
}

static void trim_two_strings_and_compare(
    char first[restrict static 1], char second[restrict static 1])
{
    mrvn_trim_string_with(first, " .");
    mrvn_trim_string_with(second, " .");

    compare_two_strings(first, second);
}

void test_text_comparison()
{
    compare_two_strings("AUX.", "Aux.");
    compare_two_strings("", "");
    compare_two_strings("ONE\nL I N E", "one\nl i n e");
    compare_two_strings("ONELINE    ", "    oneline");
    char first[] = "  ONELINE . . ";
    char second[] = " ..    oneline";
    trim_two_strings_and_compare(first, second);
    char lhs[] = "   ab   ";
    char rhs[] = " ..Ab.  ";
    trim_two_strings_and_compare(lhs, rhs);
    char one[] = "       ";
    char two[] = " .. .  ";
    trim_two_strings_and_compare(one, two);
}

void test_2(void)
{
    const struct
    {
        char* key;
        char* value;
    } temp_array[] = {{"TIT1", "Content group frame"}, {"TIT2", "Title"},
        {"TIT3", "Subtitle"}};

    const size_t n = sizeof(temp_array) / sizeof(*temp_array);
    struct mrvn_map_char* map_ptr = mrvn_map_char_new(n, 0);

    for (size_t i = 0; i < n; ++i)
    {
        mrvn_map_char_insert(map_ptr, temp_array[i].key, temp_array[i].value);
    }

    print_map_char(map_ptr);
    mrvn_map_char_delete(map_ptr);
}

int main(void)
{
    printf("Running tests.\n");
    test_1();
    test_2();

    create_dangerous_filenames();
    test_text_comparison();

    return 0;
}
