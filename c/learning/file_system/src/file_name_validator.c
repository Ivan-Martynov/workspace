#include "file_name_validator.h"
#include <ctype.h>
#include <string.h>

static const char* windows_reserved_names[] = {"AUX", "COM1", "COM2", "COM3",
    "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "CON", "LPT1", "LPT2",
    "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9", "NUL", "PRN"};

// "/?><\\:*|\"^\n";
static const char windows_reserved_characters[]
    = {'/', '?', '>', '<', '\\', ':', '*', '|', '\"', '^', '\n'};

static const size_t name_size_threshold = 256;
static const size_t windows_path_size_threshold = 247;

bool name_too_long(const char name[static 1])
{
    return strlen(name) >= name_size_threshold;
}

bool path_too_long(const char name[static 1])
{
    return strlen(name) >= windows_path_size_threshold;
}

static int compare_to_upper(
    const char text[static 1], const char text_in_uppercase[static 1])
{
    const unsigned char* lhs = (const unsigned char*)text;
    const unsigned char* rhs = (const unsigned char*)text_in_uppercase;

    while ((*lhs != '\0') && (toupper(*lhs) == *rhs))
    {
        ++lhs;
        ++rhs;
    }

    return (int)*lhs - (int)*rhs;
}

static bool equals_to_upper(
    const char text[static 1], const char text_in_uppercase[static 1])
{
    return compare_to_upper(text, text_in_uppercase) == 0;
}

bool has_invalid_characters(const char name[static 1])
{
    for (const unsigned char* ptr = (const unsigned char*)name; ptr; ++ptr)
    {
        if (iscntrl(*ptr) || strchr(windows_reserved_characters, *ptr))
        {
            return true;
        }
    }

    return false;
}

bool is_reserved_name(const char name[static 1])
{
    size_t i
        = sizeof(windows_reserved_names) / sizeof(windows_reserved_names[0])
          - 1;
    do
    {
        if (equals_to_upper(name, windows_reserved_names[i]))
        {
            return true;
        }
    } while (i--);

    return false;
}
