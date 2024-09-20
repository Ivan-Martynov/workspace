#include "FileNameValidator.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>

namespace Marvin
{

// Limitations for file/folder name length.
constexpr size_t name_size_threshold {256};
constexpr size_t windows_path_size_threshold {247};

// Forbidden names.
std::wstring_view windows_reserved_names[] {L"AUX", L"COM1", L"COM2", L"COM3",
    L"COM4", L"COM5", L"COM6", L"COM7", L"COM8", L"COM9", L"CON", L"LPT1",
    L"LPT2", L"LPT3", L"LPT4", L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9",
    L"NUL", L"PRN"};

// Forbidden characters.
static const std::wstring windows_reserved_characters {L"/?><\\:*|\"^\n"};

/**
 * @brief Check if file name is too long.
 *
 * @param file_path Item path to check.
 * @return true if name is too long, false otherwise.
 */
bool FileNameValidator::filename_too_long(const std::wstring_view& file_path)
{
    return file_path.length() > name_size_threshold;
}

/**
 * @brief Check if directory name is too long.
 *
 * @param file_path Item path to check.
 * @return true if name is too long, false otherwise.
 */
bool FileNameValidator::directory_name_too_long(
    const std::wstring_view& directory_path)
{
    return directory_path.length() > windows_path_size_threshold;
}

/**
 * @brief Check if the path is valid.
 *
 * @param file_path Path to check.
 * @return true if path is valid, false otherwise.
 */
bool FileNameValidator::is_valid(const std::wstring_view& file_path)
{
    if (filename_too_long(file_path) || directory_name_too_long(file_path))
    {
        return false;
    }

    if (file_path.find_first_of(windows_reserved_characters)
        != std::wstring::npos)
    {
        return false;
    }

    for (const auto name : windows_reserved_names)
    {
        if (std::equal(name.cbegin(), name.cend(), file_path.cbegin(),
                file_path.cend(), [](wchar_t a, wchar_t b)
                { return std::towlower(a) == std::towlower(b); }))
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Replace invalid characters in the path name.
 *
 * @param path_name Path name.
 * @param replacement Replacement for each invalid character.
 */
void FileNameValidator::replace_invalid_characters(
    std::wstring& path_name, const std::wstring& replacement)
{
    if (!is_valid(replacement))
    {
        std::wcout << L"Cannot replace invalid characters, because the "
                      L"replacement contains invalid characters. Replacing "
                      L"with underscore character.\n";
        replace_invalid_characters(path_name, L"_");
        return;
    }

    // Replace each invalid character.
    while (true)
    {
        const auto n {path_name.find_first_of(windows_reserved_characters)};
        if (n == std::wstring::npos)
        {
            break;
        }
        const auto start {path_name.cbegin() + n};
        path_name.replace(start, start + 1, replacement);
    }
}

} // namespace Marvin
