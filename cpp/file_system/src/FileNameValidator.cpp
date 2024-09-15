#include "FileNameValidator.hpp"

#include <iostream>
#include <algorithm>
#include <filesystem>
#include <regex>

namespace Marvin
{

constexpr size_t name_size_threshold {256};
constexpr size_t windows_path_size_threshold {247};

std::wstring_view windows_reserved_names[] {L"AUX", L"COM1", L"COM2", L"COM3",
    L"COM4", L"COM5", L"COM6", L"COM7", L"COM8", L"COM9", L"CON", L"LPT1",
    L"LPT2", L"LPT3", L"LPT4", L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9",
    L"NUL", L"PRN"};

//constexpr wchar_t windows_reserved_characters[] {
//    L'/', L'?', L'>', L'<', L'\\', L':', L'*', L'|', L'\"', L'^', L'\n'};
static const std::wstring windows_reserved_characters {L"/?><\\:*|\"^\n"};

static bool filename_too_long(const std::wstring_view& file_path)
{
    return file_path.length() > name_size_threshold;
}

static bool directory_name_too_long(const std::wstring_view& directory_path)
{
    return directory_path.length() > windows_path_size_threshold;
}

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

void FileNameValidator::replace_invalid_characters(
    std::wstring& path_name, const std::wstring& replacement)
{
    if (!is_valid(replacement))
    {
        std::wcout << L"Cannot replace invalid characters, because the "
                      L"replacement contains invalid characters. Replacing "
                      L"with whitespaces.\n";
        replace_invalid_characters(path_name, L"_");
        return;
    }

    // const std::wregex r(L"/|?|>|<|\\|:|*|||\"|^|\n");
    // std::regex_replace(s, r, replacement);
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
