#include "BatchFileRenamer.hpp"
#include "FileDecorator.hpp"

#include "StringReplaceCommand.hpp"
#include "CaseModifyCommand.hpp"

#include "FileNameValidator.hpp"

#include <iostream>
#include <algorithm>
#include <regex>

namespace Marvin
{

BatchFileRenamer::BatchFileRenamer(const std::vector<std::string_view>& paths,
    const std::vector<std::string_view>& options)
    : m_paths {paths}, m_options {options}
{
    for (size_t i {0}; i < m_options.size(); ++i)
    {
        const auto option {m_options[i]};
        if ((option == "-r") || (option == "-recursive"))
        {
            m_recursive = true;
        }
        else if ((option == "-v") || (option == "-verbose"))
        {
            m_verbose = true;
        }
        else if ((option == "-dm") || (option == "-do-modify"))
        {
            do_modify = true;
        }
        else if ((option == "-nf") || (option == "-no-filenames"))
        {
            m_targets = static_cast<size_t>(Targets::NONE);
        }
        else if ((option == "-d") || (option == "-directories"))
        {
            m_targets |= static_cast<size_t>(Targets::DIRECTORIES);
        }
        else if ((option == "-do") || (option == "-directories-only"))
        {
            m_targets = static_cast<size_t>(Targets::DIRECTORIES);
        }
        else if ((option == "-e") || (option == "-extensions"))
        {
            m_targets |= static_cast<size_t>(Targets::EXTENSIONS);
        }
        else if ((option == "-e") || (option == "-extensions"))
        {
            m_targets = static_cast<size_t>(Targets::EXTENSIONS);
        }
        else if ((option == "-sbn") || (option == "-sort-by-name"))
        {
            m_sorting |= static_cast<size_t>(Sorting::BY_NAMES);
        }
        else if ((option == "-sbnd") || (option == "-sort-by-name-descending"))
        {
            m_sorting |= static_cast<size_t>(Sorting::BY_NAMES_DESCENDING);
        }
        else if ((option == "-sbs") || (option == "-sort-by-size"))
        {
            m_sorting |= static_cast<size_t>(Sorting::BY_SIZE);
        }
        else if ((option == "-sbsd") || (option == "-sort-by-size-descending"))
        {
            m_sorting |= static_cast<size_t>(Sorting::BY_SIZE_DESCENDING);
        }
        else if ((option == "-sbt") || (option == "-sort-by-timestamp"))
        {
            m_sorting |= static_cast<size_t>(Sorting::BY_TIMESTAMP);
        }
        else if ((option == "-sbtd")
                 || (option == "-sort-by-timestamp-descending"))
        {
            m_sorting |= static_cast<size_t>(Sorting::BY_TIMESTAMP_DESCENDING);
        }
        else if ((option == "-rur") || (option == "-replace-using-regex")
            || (option == "-rsw") || (option == "-replace-substring-with"))
        {
            std::wstring match {L""};
            std::wstring replacement {L""};
            if (++i < m_options.size())
            {
                // A trick to convert string to wstring.
                match = std::filesystem::path(m_options[i]).wstring();
            }

            if (++i < m_options.size())
            {
                replacement = std::filesystem::path(m_options[i]).wstring();
            }

            if (!match.empty())
            {
                m_commands_ptrs.emplace_back(
                    std::make_unique<StringReplaceCommand>(match, replacement));
            }
        }
        else if ((option == "-rww") || (option == "-replace-whitespace-with"))
        {
            std::wstring replacement {L""};
            if (++i < m_options.size())
            {
                replacement = std::filesystem::path(m_options[i]).wstring();
            }
            m_commands_ptrs.emplace_back(
                std::make_unique<StringReplaceCommand>(L"(\\s+)", replacement));
        }
        else if ((option == "-rwwu")
                 || (option == "-replace-whitespace-with-underscore"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<StringReplaceCommand>(L"(\\s+)", L"_"));
        }
        else if ((option == "-tu") || (option == "-to-uppercase"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<CaseModifyCommand>('u'));
        }
        else if ((option == "-tl") || (option == "-to-lowercase"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<CaseModifyCommand>('l'));
        }
        else if ((option == "-tc") || (option == "-to-camelcase"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<CaseModifyCommand>('c'));
        }
    }
}

void BatchFileRenamer::process_items(std::vector<std::filesystem::path>& items)
{
    for (std::filesystem::path& path : items)
    {
        const auto original_path {path};
        for (auto& command_ptr : m_commands_ptrs)
        {
            // std::wcout << L"Trying to modify path\n";
            command_ptr->modify(path);
        }

        auto s = path.filename().wstring();
        FileNameValidator::replace_invalid_characters(s, L" ");
        path.replace_filename(s);

        std::wcout << L"Final path: " << path << L"\n";

        if (do_modify)
        {
            std::error_code err_code {};
            std::filesystem::rename(original_path, path, err_code);
            if (err_code)
            {
                std::wcerr << L"what(): "
                           << std::filesystem::path(err_code.message())
                           << L"\n";
            }
        }
    }
}

void BatchFileRenamer::process_directory(const std::string_view& directory_path)
{
    std::vector<std::filesystem::path> files {};
    std::vector<std::filesystem::path> directories {};

    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {directory_path})
    {
        if (item.is_directory())
        {
            if (m_recursive)
            {
                process_directory(item.path().string());
            }

            if (m_targets & static_cast<size_t>(Targets::DIRECTORIES))
            {
                directories.emplace_back(item);
            }
        }
        else if (item.is_regular_file()
                 && (m_targets & static_cast<size_t>(Targets::FILES)))
        {
            files.emplace_back(item);
        }
    }

    // Sort by names to have unique sorting if the flags is set.
    if ((m_sorting & static_cast<size_t>(Sorting::BY_NAMES))
        || (m_sorting & static_cast<size_t>(Sorting::BY_NAMES_DESCENDING)))
    {
        if ((m_sorting & static_cast<size_t>(Sorting::BY_NAMES)))
        {
            std::sort(files.begin(), files.end());
            std::sort(directories.begin(), directories.end());
        }
        else
        {
            const auto f {std::greater<>()};

            std::sort(files.begin(), files.end(), f);
            std::sort(directories.begin(), directories.end(), f);
        }
    }

    // Sort by file size to have unique sorting if the flags is set. Using
    // stable sort to preserve order. Directories are not sorted by size.
    if ((m_sorting & static_cast<size_t>(Sorting::BY_SIZE))
        || (m_sorting & static_cast<size_t>(Sorting::BY_SIZE_DESCENDING)))
    {
        const bool ascending {
            (m_sorting & static_cast<size_t>(Sorting::BY_SIZE_DESCENDING))
            == 0};
        std::sort(files.begin(), files.end(),
            [&ascending](auto& a, auto& b)
            {
                std::error_code err_code {};
                const auto size_a {std::filesystem::file_size(a, err_code)};
                if (err_code)
                {
                    return ascending;
                }
                const auto size_b {std::filesystem::file_size(b, err_code)};
                if (err_code)
                {
                    return !ascending;
                }
                return ascending ? size_a < size_b : size_a > size_b;
            });
    }

    // Sort by timestamp to have unique sorting if the flags is set. Using
    // stable sort to preserve order.
    if ((m_sorting & static_cast<size_t>(Sorting::BY_TIMESTAMP))
        || (m_sorting & static_cast<size_t>(Sorting::BY_TIMESTAMP_DESCENDING)))
    {
        const bool ascending {
            (m_sorting & static_cast<size_t>(Sorting::BY_TIMESTAMP_DESCENDING))
            == 0};
        auto f {[&ascending](auto& a, auto& b)
            {
                std::error_code err_code {};
                const auto stamp_a {
                    std::filesystem::last_write_time(a, err_code)};
                if (err_code)
                {
                    return ascending;
                }
                const auto stamp_b {
                    std::filesystem::last_write_time(b, err_code)};
                if (err_code)
                {
                    return !ascending;
                }

                return ascending ? stamp_a < stamp_b : stamp_a > stamp_b;
            }};
    }

    process_items(files);
    process_items(directories);
}

void BatchFileRenamer::run()
{
    for (const auto& directory_path: m_paths)
    {
        process_directory(directory_path);
    }
}

} // namespace Marvin
