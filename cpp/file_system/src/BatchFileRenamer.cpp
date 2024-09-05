#include "BatchFileRenamer.hpp"
#include "FileDecorator.hpp"

#include <iostream>
#include <algorithm>
#include <regex>

namespace Marvin
{

BatchFileRenamer::BatchFileRenamer(const std::vector<std::string>& paths,
    const bool is_recursive, const bool is_verbose)
    : m_recursive {is_recursive}, m_verbose {is_verbose}
{
    for (const std::string& dir_path : paths)
    {
        if (m_recursive)
        {
            for (const std::filesystem::directory_entry& item :
                std::filesystem::recursive_directory_iterator {dir_path,
                    std::filesystem::directory_options::skip_permission_denied})
            {
                entries.emplace_back(item);
            }
        }
        else
        {
            for (const std::filesystem::directory_entry& item :
                std::filesystem::directory_iterator {dir_path,
                    std::filesystem::directory_options::skip_permission_denied})
            {
                entries.emplace_back(item);
            }
        }
    }
}

void BatchFileRenamer::replace_using_regex(
    const std::wstring& search_for, const std::wstring& replacement)
{
    for (auto& item : entries)
    {
        const auto p {item.path()};

        const auto file_name {std::regex_replace(
            p.filename().wstring(), std::wregex(search_for), replacement)};
        const auto modified_path {p.parent_path() / file_name};

        const bool no_change {p == modified_path};

        if (m_verbose && !no_change)
        {
            std::wcout << p << L" => " << modified_path;
            if (no_change)
            {
                std::wcout << L" (no change)";
            }
            std::wcout << L"\n";
        }

        if (!no_change && !m_verbose)
        {
            try
            {
                //std::filesystem::rename(p, modified_path);
            }
            catch (const std::filesystem::filesystem_error& err)
            {
                std::cerr << "what(): " << err.what() << "\n";
            }
        }
    }
}

void BatchFileRenamer::replace_spaces_with_underscores()
{
    replace_using_regex(L"(\\s+)", L"_");
}

} // namespace Marvin
