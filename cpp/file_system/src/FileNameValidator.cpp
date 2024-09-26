#include "FileNameValidator.h"
#include "StringHelper.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>

namespace Marvin
{

// Limitations for file/folder name length.
static constexpr size_t max_file_name_length {255};
// Directory length has a bit strange looking number, however 247 + 8 = 255,
// therefore, perhaps, that value of 8 is reserved for specific purposes.
static constexpr size_t max_directory_name_length {247};

// Forbidden names.
static constexpr std::wstring_view windows_reserved_names[] {L"AUX", L"COM0",
    L"COM1", L"COM2", L"COM3", L"COM4", L"COM5", L"COM6", L"COM7", L"COM8",
    L"COM9", L"COM¹", L"COM²", L"COM³", L"CON", L"LPT0", L"LPT1", L"LPT2",
    L"LPT3", L"LPT4", L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9", L"LPT¹",
    L"LPT²", L"LPT³", L"NUL", L"PRN"};

// Forbidden characters.
static constexpr std::wstring_view forbidden_characters {L"/?><\\:*|\"^"};

/**
 * @brief Check whether the name contains at least one invalid character.
 * 
 * @param[in] file_name Name to check.
 * @return true The name contains at least one invalid character.
 * @return false otherwise.
 */
bool FileNameValidator::contains_invalid_character(
    const std::wstring_view& file_name)
{
    for (const auto& c: file_name)
    {
        if (!std::iswprint(c) || forbidden_characters.contains(c))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Check if the file name ends with a period (dot) or space.
 *
 * @param[in] file_name Name to check.
 * @return true if the file name ends with a period (dot) or space.
 * @return false otherwise.
 */
bool FileNameValidator::ends_with_space_or_period(
    const std::wstring_view& file_name)
{
    return std::wstring_view {L" ."}.contains(file_name.back());
}

/**
 * @brief Check if file name is too long.
 *
 * @param file_name Item path to check.
 * @return true if name is too long, false otherwise.
 */
bool FileNameValidator::filename_too_long(const std::wstring_view& file_name)
{
    return !std::filesystem::is_directory(file_name)
           && (file_name.length() > max_file_name_length);
}

/**
 * @brief Check if directory name is too long.
 *
 * @param file_name Item path to check.
 * @return true if name is too long, false otherwise.
 */
bool FileNameValidator::directory_name_too_long(
    const std::wstring_view& directory_path)
{
    return std::filesystem::is_directory(directory_path)
           && (directory_path.length() > max_directory_name_length);
}

/**
 * @brief Check if the path is valid.
 *
 * @param file_name Path to check.
 * @return true if path is valid, false otherwise.
 */
bool FileNameValidator::is_valid(const std::wstring_view& file_name)
{
    if (file_name.empty() || (file_name == L".") || (file_name == L".."))
    {
        return false;
    }

    if (filename_too_long(file_name) || directory_name_too_long(file_name))
    {
        return false;
    }

    if (contains_invalid_character(file_name))
    {
        return false;
    }

    for (const auto name : windows_reserved_names)
    {
        if (StringHelper::equal_ignore_case(name, file_name))
        {
            return false;
        }
    }

    return !ends_with_space_or_period(file_name);
}

/**
 * @brief Try fixing file name by replacing invalid characters and trimming
 * if needed.
 *
 * @param[in] file_name Path name.
 * @param[in] replacement Replacement for each invalid character.
 * @return true If name is modified into a valid one, false otherwise.
 */
bool FileNameValidator::try_fixing_name(
    std::wstring& file_name, const std::wstring& replacement)
{
    if (contains_invalid_character(replacement))
    {
        std::wcerr << L"Cannot replace invalid characters, because the "
                      L"replacement contains invalid characters. Replacing "
                      L"with underscore character.\n";
        return try_fixing_name(file_name, L"_");
    }

    // Replace each invalid character.
    while (true)
    {
        const auto n {file_name.find_first_of(forbidden_characters)};
        if (n == std::wstring::npos)
        {
            break;
        }
        const auto start {file_name.cbegin() + n};
        file_name.replace(start, start + 1, replacement);
    }

    while (!file_name.empty() && ends_with_space_or_period(file_name))
    {
        file_name.pop_back();
    }

    return is_valid(file_name);
}

} // namespace Marvin
