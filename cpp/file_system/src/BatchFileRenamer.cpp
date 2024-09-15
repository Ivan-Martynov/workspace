#include "BatchFileRenamer.hpp"

#include "StringHelper.hpp"
#include "StringReplaceCommand.hpp"
#include "CaseModifyCommand.hpp"
#include "StringAppendCommand.hpp"
#include "FileNameValidator.hpp"

#include <iostream>
#include <algorithm>
#include <regex>
#include <utility>

namespace Marvin
{

/**
 * @brief Construct a new Batch File Renamer:: Batch File Renamer object.
 * 
 * @param paths Paths to process.
 * @param options Options to use for processing.
 */
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
        else if ((option == "-dow") || (option == "-do-overwrite"))
        {
            m_do_overwrite = Overwrite::YES;
        }
        else if ((option == "-dnow") || (option == "-do-not-overwrite"))
        {
            m_do_overwrite = Overwrite::NO;
        }
        else if ((option == "-nf") || (option == "-no-filenames"))
        {
            m_targets = std::to_underlying(Targets::NONE);
        }
        else if ((option == "-d") || (option == "-directories"))
        {
            m_targets |= std::to_underlying(Targets::DIRECTORIES);
        }
        else if ((option == "-do") || (option == "-directories-only"))
        {
            m_targets = std::to_underlying(Targets::DIRECTORIES);
        }
        else if ((option == "-e") || (option == "-extensions"))
        {
            m_targets |= std::to_underlying(Targets::EXTENSIONS);
        }
        else if ((option == "-eo") || (option == "-extensions-only"))
        {
            m_targets = std::to_underlying(Targets::EXTENSIONS);
        }
        else if ((option == "-sbn") || (option == "-sort-by-name"))
        {
            m_sorting |= std::to_underlying(Sorting::BY_NAMES);
        }
        else if ((option == "-sbnd") || (option == "-sort-by-name-descending"))
        {
            m_sorting |= std::to_underlying(Sorting::BY_NAMES_DESCENDING);
        }
        else if ((option == "-sbs") || (option == "-sort-by-size"))
        {
            m_sorting |= std::to_underlying(Sorting::BY_SIZE);
        }
        else if ((option == "-sbsd") || (option == "-sort-by-size-descending"))
        {
            m_sorting |= std::to_underlying(Sorting::BY_SIZE_DESCENDING);
        }
        else if ((option == "-sbt") || (option == "-sort-by-timestamp"))
        {
            m_sorting |= std::to_underlying(Sorting::BY_TIMESTAMP);
        }
        else if ((option == "-sbtd")
                 || (option == "-sort-by-timestamp-descending"))
        {
            m_sorting |= std::to_underlying(Sorting::BY_TIMESTAMP_DESCENDING);
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
        else if ((option == "-an") || (option == "-append-number"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<StringAppendCommand>('a', 'n'));
        }
        else if ((option == "-pn") || (option == "-prepend-number"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<StringAppendCommand>('p', 'n'));
        }
        else if ((option == "-at") || (option == "-append-timestamp"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<StringAppendCommand>('a', 't'));
        }
        else if ((option == "-pt") || (option == "-prepend-timestamp"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<StringAppendCommand>('p', 't'));
        }
        else if ((option == "-act") || (option == "-append-current-time"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<StringAppendCommand>('a', 'c'));
        }
        else if ((option == "-pct") || (option == "-prepend-current-time"))
        {
            m_commands_ptrs.emplace_back(
                std::make_unique<StringAppendCommand>('p', 'c'));
        }
    }
}

void BatchFileRenamer::process_items(
    std::vector<std::filesystem::path>& items, const bool directories)
{
    std::vector<std::wstring> original_paths {};
    for (size_t i {0}; i < items.size(); ++i)
    {
        original_paths.emplace_back(items[i].wstring());
    }

    auto target {std::to_underlying(FileRenameCommandInterface::Target::STEM)};
    if (!directories)
    {
        if (m_targets == std::to_underlying(Targets::EXTENSIONS))
        {
            // Only extension.
            target = std::to_underlying(
                FileRenameCommandInterface::Target::EXTENSION);
        }
        else if (m_targets & std::to_underlying(Targets::EXTENSIONS))
        {
            // Stem and extension.
            target |= std::to_underlying(
                FileRenameCommandInterface::Target::EXTENSION);
        }
    }

    for (auto& command_ptr : m_commands_ptrs)
    {
        command_ptr->modify(items, target);
    }

    for (size_t i {0}; i < items.size(); ++i)
    {
        std::wstring file_name {items[i].filename().wstring()};
        FileNameValidator::replace_invalid_characters(file_name, L"_");

        if (file_name.empty())
        {
            if (m_verbose)
            {
                std::wcerr << L"New file name is empty for "
                           << original_paths[i] << L'\n';
            }
            continue;
        }

        if (directories && ((file_name == L".") || (file_name == L"..")))
        {
            std::wcerr << L"Cannot rename a directory into the current or "
                          L"upper directory\n";
            return;
        }

        std::filesystem::path new_path {items[i].parent_path() / file_name};

        const bool is_same {original_paths[i] == new_path};
        if (m_verbose)
        {
            std::wcout << original_paths[i] << L"=>" << new_path
                       << (is_same ? L" (no change)\n" : L"\n");
        }

        if (is_same)
        {
            continue;
        }

        if (do_modify)
        {

            if (std::filesystem::exists(items[i]))
            {
                std::wcout << L"File already exists: " << items[i] << L'\n';
                bool do_overwrite {m_do_overwrite == Overwrite::YES};
                if (!do_overwrite && (m_do_overwrite != Overwrite::NO))
                {
                    std::wcout << "Overwrite? [n]o, [y]es, yes to [a]ll, n[o] "
                                  "to all\n";
                    char answer {};
                    std::cin >> answer;
                    switch (answer)
                    {
                        case 'a':
                            m_do_overwrite = Overwrite::YES;
                            do_overwrite = true;
                            break;

                        case 'y':
                            do_overwrite = true;
                            break;

                        case 'o':
                            m_do_overwrite = Overwrite::NO;
                            break;

                        default:
                            break;
                    }
                }

                if (!do_overwrite)
                {
                    continue;
                }
            }

            std::error_code err_code {};
            std::filesystem::rename(original_paths[i], new_path, err_code);
            if (err_code)
            {
                std::wcerr << L"what(): "
                           << StringHelper::string_to_wstring(
                                  err_code.message())
                           << L"\n";
            }
        }
    }
}

void BatchFileRenamer::check_sorting_by_names(
    std::vector<std::filesystem::path>& files,
    std::vector<std::filesystem::path>& directories)
{
    // Sort by names to have unique sorting if the flags is set.
    if ((m_sorting & std::to_underlying(Sorting::BY_NAMES))
        || (m_sorting & std::to_underlying(Sorting::BY_NAMES_DESCENDING)))
    {
        if ((m_sorting & std::to_underlying(Sorting::BY_NAMES)))
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
}

void BatchFileRenamer::check_sorting_by_size(
    std::vector<std::filesystem::path>& files)
{
    if ((m_sorting & std::to_underlying(Sorting::BY_SIZE))
        || (m_sorting & std::to_underlying(Sorting::BY_SIZE_DESCENDING)))
    {
        const bool ascending {
            (m_sorting & std::to_underlying(Sorting::BY_SIZE_DESCENDING)) == 0};
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
}

void BatchFileRenamer::check_sorting_by_timestamp(
    std::vector<std::filesystem::path>& files,
    std::vector<std::filesystem::path>& directories)
{
    if ((m_sorting & std::to_underlying(Sorting::BY_TIMESTAMP))
        || (m_sorting & std::to_underlying(Sorting::BY_TIMESTAMP_DESCENDING)))
    {
        const bool ascending {
            (m_sorting & std::to_underlying(Sorting::BY_TIMESTAMP_DESCENDING))
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
        std::sort(files.begin(), files.end(), f);
        std::sort(directories.begin(), directories.end(), f);
    }
}

void BatchFileRenamer::process_directory(const std::string_view& directory_path)
{
    std::vector<std::filesystem::path> files {};
    std::vector<std::filesystem::path> directories {};

    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {directory_path})
    {
        if (item.is_directory() && (item.path() != ".")
            && (item.path() != ".."))
        {
            if (m_recursive)
            {
                process_directory(item.path().string());
            }

            if (m_targets & std::to_underlying(Targets::DIRECTORIES))
            {
                directories.emplace_back(item);
            }
        }
        else if (item.is_regular_file()
                 && (m_targets & std::to_underlying(Targets::FILES)))
        {
            files.emplace_back(item);
        }
    }

    // Sort by names to have unique sorting if the flags is set.
    check_sorting_by_names(files, directories);

    // Sort by file size to have unique sorting if the flags is set. Using
    // stable sort to preserve order. Directories are not sorted by size.
    check_sorting_by_size(files);

    // Sort by timestamp to have unique sorting if the flags is set. Using
    // stable sort to preserve order.
    check_sorting_by_timestamp(files, directories);

    process_items(files, false);
    process_items(directories, true);
}

void BatchFileRenamer::run()
{
    for (const auto& directory_path : m_paths)
    {
        process_directory(directory_path);
    }
}

} // namespace Marvin
