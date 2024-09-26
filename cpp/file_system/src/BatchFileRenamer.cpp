#include "BatchFileRenamer.h"

#include "CaseModifyCommand.h"
#include "FileNameValidator.h"
#include "StringAppendCommand.h"
#include "StringHelper.h"
#include "StringReplaceCommand.h"

#include <algorithm>
#include <iostream>
#include <utility>

namespace Marvin
{

/**
 * @brief Set current option.
 *
 * @param[in] i Index of the item in the list of options - might be
 * modified.
 */
void BatchFileRenamer::m_set_option(size_t& i)
{
    const auto option {m_options[i]};
    if ((option == "-h") || (option == "-help"))
    {
        m_do_show_help = true;
    }
    else if ((option == "-r") || (option == "-recursive"))
    {
        m_recursive = true;
    }
    else if ((option == "-v") || (option == "-verbose"))
    {
        m_verbose = true;
    }
    else if ((option == "-dm") || (option == "-do-modify"))
    {
        m_do_modify = true;
    }
    else if ((option == "-dow") || (option == "-do-overwrite"))
    {
        m_overwrite_prompt.set_mode(FileOverwritePrompt::Mode::YES_TO_ALL);
    }
    else if ((option == "-dowu") || (option == "-do-overwrite-update"))
    {
        m_overwrite_prompt.set_mode(
            FileOverwritePrompt::Mode::YES_TO_ALL_UPDATE);
    }
    else if ((option == "-dnow") || (option == "-do-not-overwrite"))
    {
        m_overwrite_prompt.set_mode(FileOverwritePrompt::Mode::NO_TO_ALL);
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
    else if ((option == "-sbtd") || (option == "-sort-by-timestamp-descending"))
    {
        m_sorting |= std::to_underlying(Sorting::BY_TIMESTAMP_DESCENDING);
    }
    else if ((option == "-c") || (option == "-clear"))
    {
        m_commands_ptrs.emplace_back(
            std::make_unique<StringReplaceCommand>(L".", L""));
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
        m_commands_ptrs.emplace_back(std::make_unique<CaseModifyCommand>('u'));
    }
    else if ((option == "-tl") || (option == "-to-lowercase"))
    {
        m_commands_ptrs.emplace_back(std::make_unique<CaseModifyCommand>('l'));
    }
    else if ((option == "-tc") || (option == "-to-camelcase"))
    {
        m_commands_ptrs.emplace_back(std::make_unique<CaseModifyCommand>('c'));
    }
    else if ((option == "-tp") || (option == "-to-pascalcase"))
    {
        m_commands_ptrs.emplace_back(std::make_unique<CaseModifyCommand>('p'));
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
        m_set_option(i);
    }
}

/**
 * @brief Show help information.
 */
void BatchFileRenamer::m_show_help() const
{
    std::wcout
        << L"\nThe program expects file paths and options. By default the "
           L"files are not renamed - the program only defines how the "
           L"renamed file might look like. The options:\n"
           L"-\nh or -help: show this help information;\n"
           L"\n-r or -recursive: process directories recursively;\n"
           L"\n-v or -verbose: output information about modified name to "
           L"the terminal;\n"
           L"\n-dm or -do-modify: do the actual renaming of the file "
           L"(WARNING: make sure you know what you are doing);\n"
           L"\n-dow or -do-overwrite: overwrite existing items if the new "
           L"path names match the existing item (WARNING: make sure you "
           L"know what you are doing);\n"
           L"\n-dowu or -do-overwrite-update: overwrite existing items if the "
           L"new path names match the existing item and the new file is newer "
           L"(WARNING: make sure you know what you are doing);\n"
           L"\n-dnow or -do-not-overwrite: do not overwrite existing "
           L"items if the new path names match the existing item;\n"
           L"\n-nf or -no-filenames: do not process file names;\n"
           L"\n-d or -directories: process directories too;\n"
           L"\n-do or -directories-only: process directories only;\n"
           L"\n-e or -extensions: process extensions too;\n"
           L"\n-eo or -extensions-only: process extensions only;\n"
           L"\n-sbn or -sort-by-name: sort items by name;\n"
           L"\n-sbnd or -sort-by-name-descending: sort items by name in "
           L"descending order;\n"
           L"\n-sbs or -sort-by-size: sort items by size (currently only "
           L"affecting files, that is not sorting directories by size);\n"
           L"\n-sbsd or -sort-by-size-descending: sort items by size in "
           L"descending order (currently only affecting files, that is not "
           L"sorting directories by size);\n"
           L"\n-sbt or -sort-by-timestamp: sort items by timestamp (when it "
           L"was "
           L"last modified);\n"
           L"\n-sbtd or -sort-by-timestamp-descending: sort items by timestamp "
           L"in descending order (when it was last modified);\n"
           L"\n-c or -clear: clear the content\n"
           L"\n-rur or -replace-using-regex: replace substring represented as "
           L"a "
           L"regular expression with a replacement. Must be followed by two "
           L"values: the expression to match and a replacement string. For "
           L"example, -replace-using-regex \"[0-9]\" "
           " to remove all digits. WARNING, not all regular expressions can be "
           "used. For instance, \\ character (backslash) can trigger "
           "terminal\'s special state, stopping the execution;\n"
           L"\n-rsw or -replace-substring-with: replace substring with a "
           L"replacement. Must be followed by two values: the text to match "
           L"and a replacement string;\n"
           L"\n-rww or -replace-whitespace-with: replace whitespaces with a "
           L"replacement. Must be followed by a replacement string;\n"
           L"\n-rwwu or -replace-whitespace-with-underscore: replace "
           L"whitespaces "
           L"with underscores;\n"
           L"\n-tu or -to-uppercase: change item name to uppercase;\n"
           L"\n-tl or -to-lowercase: change item name to lowercase;\n"
           L"\n-tc or -to-camelcase: change item name to camelcase;\n"
           L"\n-an or -append-number: append numbers sequentially to items. "
           L"Typically used to order files or folders.\n"
           L"\n-pn or -prepend-number: prepend numbers sequentially to items;\n"
           L"\n-at or -append-timestamp: append item\'s timestamp to the "
           L"item;\n"
           L"\n-pt or -prepend-timestamp: prepend item\'s timestamp to the "
           L"item;\n"
           L"\n-act or -append-current-time: append current time to the item;\n"
           L"\n-pct or -prepend-current-time: prepend current time to the "
           L"item;\n";
}

size_t BatchFileRenamer::m_build_target_flag(const bool are_directories) const
{
    // Build the target flag to process the desired items parts (i.e., file
    // names and/or their extensions).
    size_t target {std::to_underlying(FileRenameCommandBase::Target::STEM)};
    if (!are_directories)
    {
        if (m_targets == std::to_underlying(Targets::EXTENSIONS))
        {
            // Only extension.
            target
                = std::to_underlying(FileRenameCommandBase::Target::EXTENSION);
        }
        else if (m_targets & std::to_underlying(Targets::EXTENSIONS))
        {
            // Stem and extension.
            target
                |= std::to_underlying(FileRenameCommandBase::Target::EXTENSION);
        }
    }
    return target;
}

/**
 * @brief Process items (either files or directories).
 *
 * @param items Items to process.
 * @param are_directories Boolean flag whether the items are directories.
 */
void BatchFileRenamer::m_process_items(
    std::vector<std::pair<std::wstring, std::filesystem::path>>& items,
    const bool are_directories)
{
    // Build the target flag to process the desired items parts (i.e., file
    // names and/or their extensions).
    auto target {m_build_target_flag(are_directories)};

    // Actuall processing using renaming commands.
    for (auto& command_ptr : m_commands_ptrs)
    {
        command_ptr->modify(items, target);
    }

    for (size_t i {0}; i < items.size(); ++i)
    {
        // Check if the modified name has invalid characters and replace them if
        // any.
        std::wstring file_name {items[i].second.filename().wstring()};
        if (!FileNameValidator::try_fixing_name(file_name, L"_"))
        {
            continue;
        }

        // Build the full path for the modified item.
        std::filesystem::path new_path {
            items[i].second.parent_path() / file_name};

        if (m_verbose)
        {
            std::wcout << items[i].first << L" => " << new_path << L"\n";
        }

        // Try to actually modify (rename) the item.
        // If a modified item points to an existing file, then prompt for
        // permission to overwrite.
        if (m_do_modify
            && m_overwrite_prompt.is_overwriting(items[i].first, new_path))
        {
            // Try renaming the item and report if an error occurs.
            std::error_code err_code {};
            std::filesystem::rename(items[i].first, new_path, err_code);
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

/**
 * @brief Sorting items by name.
 *
 * @param files File names to sort.
 * @param directories Folder names to sort.
 */
void BatchFileRenamer::m_check_sorting_by_names(
    std::vector<std::pair<std::wstring, std::filesystem::path>>& files,
    std::vector<std::pair<std::wstring, std::filesystem::path>>& directories)
{
    // Sort by names to have unique sorting if the flags is set.
    if ((m_sorting & std::to_underlying(Sorting::BY_NAMES))
        || (m_sorting & std::to_underlying(Sorting::BY_NAMES_DESCENDING)))
    {
        if ((m_sorting & std::to_underlying(Sorting::BY_NAMES)))
        {
            auto compare_lambda {[](const auto& a, const auto& b)
                { return a.second < b.second; }};
            std::sort(files.begin(), files.end(), compare_lambda);
            std::sort(directories.begin(), directories.end(), compare_lambda);
        }
        else
        {
            auto compare_lambda {[](const auto& a, const auto& b)
                { return a.second > b.second; }};
            std::sort(files.begin(), files.end(), compare_lambda);
            std::sort(directories.begin(), directories.end(), compare_lambda);
        }
    }
}

/**
 * @brief Sorting items by size. Folders are not processed.
 *
 * @param files File names to sort.
 */
void BatchFileRenamer::m_check_sorting_by_size(
    std::vector<std::pair<std::wstring, std::filesystem::path>>& files)
{
    if ((m_sorting & std::to_underlying(Sorting::BY_SIZE))
        || (m_sorting & std::to_underlying(Sorting::BY_SIZE_DESCENDING)))
    {
        const bool ascending {
            (m_sorting & std::to_underlying(Sorting::BY_SIZE_DESCENDING)) == 0};
        std::sort(files.begin(), files.end(),
            [&ascending](
                const auto& a, const auto& b) // Lambda to be used for sorting.
            {
                std::error_code err_code {};
                const auto size_a {
                    std::filesystem::file_size(a.second, err_code)};
                if (err_code)
                {
                    // If failed to get the size of the first item, then assume
                    // it to be smaller.
                    return ascending ? a.second < b.second
                                     : a.second > b.second;
                }
                const auto size_b {
                    std::filesystem::file_size(b.second, err_code)};
                if (err_code)
                {
                    // If failed to get the size of the second item, then sort
                    // by default order.
                    return ascending ? a.second < b.second
                                     : a.second > b.second;
                }
                return ascending ? size_a < size_b : size_a > size_b;
            });
    }
}

/**
 * @brief Sorting items by timestamp.
 *
 * @param files File names to sort.
 * @param directories Folder names to sort.
 */
void BatchFileRenamer::m_check_sorting_by_timestamp(
    std::vector<std::pair<std::wstring, std::filesystem::path>>& files,
    std::vector<std::pair<std::wstring, std::filesystem::path>>& directories)
{
    if ((m_sorting & std::to_underlying(Sorting::BY_TIMESTAMP))
        || (m_sorting & std::to_underlying(Sorting::BY_TIMESTAMP_DESCENDING)))
    {
        const bool ascending {
            (m_sorting & std::to_underlying(Sorting::BY_TIMESTAMP_DESCENDING))
            == 0};
        // Lambda to be used for sorting.
        auto compare_lambda {[&ascending](const auto& a, const auto& b)
            {
                std::error_code err_code {};
                const auto stamp_a {
                    std::filesystem::last_write_time(a.second, err_code)};
                if (err_code)
                {
                    // If failed to get the timestamp of the first item, then
                    // assume it to be smaller.
                    return ascending;
                }
                const auto stamp_b {
                    std::filesystem::last_write_time(b.second, err_code)};
                if (err_code)
                {
                    // If failed to get the timestamp of the second item, then
                    // sort by default order.
                    return ascending ? a.second < b.second : a.second > b.second;
                }

                return ascending ? stamp_a < stamp_b : stamp_a > stamp_b;
            }};
        std::sort(files.begin(), files.end(), compare_lambda);
        std::sort(directories.begin(), directories.end(), compare_lambda);
    }
}

/**
 * @brief Process items in the directory or the file path.
 *
 * @param target_path Path to the directory or file.
 */
void BatchFileRenamer::m_process_target(const std::string_view& target_path)
{
    // Keep original path and the path to modify in a pair: first element is the
    // original path name and the second is the path to work on.
    std::vector<std::pair<std::wstring, std::filesystem::path>> file_pairs {};
    std::vector<std::pair<std::wstring, std::filesystem::path>> folder_pairs {};

    auto add_pair_lambda {
        [](const std::filesystem::directory_entry& item,
            std::vector<std::pair<std::wstring, std::filesystem::path>>& pair)
        {
            auto path {item.path()};
            path.make_preferred();
            pair.emplace_back(path.wstring(), path);
        }};

    if (std::filesystem::is_directory(target_path))
    {
        // Collect file names and folder names into vectors.
        for (const std::filesystem::directory_entry& item :
            std::filesystem::directory_iterator {target_path})
        {
            // Skip current and parent directories.
            if (item.is_directory() && (item.path() != ".")
                && (item.path() != ".."))
            {
                // Process subdirectories if the corresponding flag is set.
                if (m_recursive)
                {
                    m_process_target(item.path().string());
                }

                if (m_targets & std::to_underlying(Targets::DIRECTORIES))
                {
                    add_pair_lambda(item, folder_pairs);
                }
            }
            else if ((m_targets & std::to_underlying(Targets::FILES))
                     || (m_targets & std::to_underlying(Targets::EXTENSIONS)))
            {
                add_pair_lambda(item, file_pairs);
            }
        }
    }
    else if ((m_targets & std::to_underlying(Targets::FILES))
             || (m_targets & std::to_underlying(Targets::EXTENSIONS)))
    {
        std::filesystem::path path {target_path};
        path.make_preferred();
        file_pairs.emplace_back(path.wstring(), path);
    }

    // Sort by names to have unique sorting if the flags is set.
    m_check_sorting_by_names(file_pairs, folder_pairs);

    // Sort by file size to have unique sorting if the flags is set. Using
    // stable sort to preserve order. Directories are not sorted by size.
    m_check_sorting_by_size(file_pairs);

    // Sort by timestamp to have unique sorting if the flags is set. Using
    // stable sort to preserve order.
    m_check_sorting_by_timestamp(file_pairs, folder_pairs);

    // Process file names and folder names.
    m_process_items(file_pairs, false);
    m_process_items(folder_pairs, true);
}

/**
 * @brief Process for each directory
 */
void BatchFileRenamer::run()
{
    if (m_do_show_help || m_paths.empty() || m_options.empty())
    {
        m_show_help();
    }

    for (const auto& directory_path : m_paths)
    {
        m_process_target(directory_path);
    }
}

} // namespace Marvin
