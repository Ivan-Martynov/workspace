#include "MP3BatchFileRenamer.h"
#include "MP3ReplaceCommand.h"
#include "FileNameValidator.h"
#include "StringHelper.h"

#include <iostream>

namespace Marvin
{

void MP3BatchFileRenamer::m_process_options()
{
    for (size_t i {0}; i < m_options.size(); ++i)
    {
        const auto& option {m_options[i]};
        if ((option == "-ut") || (option == "-use-title"))
        {
            m_target = std::to_underlying(Target::TITLE);
        }
        else if ((option == "-uA") || (option == "-use-artist"))
        {
            m_target = std::to_underlying(Target::ARTIST);
        }
        else if ((option == "-ua") || (option == "-use-album"))
        {
            m_target = std::to_underlying(Target::ALBUM);
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
    }
}

void MP3BatchFileRenamer::m_add_if_valid(std::vector<MP3Tag>& items,
    const std::filesystem::directory_entry& item) const
{
    MP3Tag mp3_tag {item, m_options};
    if (mp3_tag.is_valid())
    {
        items.emplace_back(mp3_tag);
    }
}

void MP3BatchFileRenamer::m_process_items(const std::string_view& path)
{
    std::vector<MP3Tag> items {};

    if (m_recursive)
    {
        for (const std::filesystem::directory_entry& item :
            std::filesystem::recursive_directory_iterator {path})
        {
            m_add_if_valid(items, item);
        }
    }
    else
    {
        for (const std::filesystem::directory_entry& item :
            std::filesystem::directory_iterator {path})
        {
            m_add_if_valid(items, item);
        }
    }

    for (auto& item : items)
    {
        const auto file_path {item.get_path()};

        std::wcout << L"Processing item " << file_path << L"\n";
        std::wstring modified_name {L""};
        // item.show_frames();
        if (m_target & std::to_underlying(Target::TITLE))
        {
            modified_name = item.get_title();
            std::wcout << L"Title = " << item.get_title() << L"\n";
        }
        else if (m_target & std::to_underlying(Target::ARTIST))
        {
            modified_name = item.get_artist();
            std::wcout << L"Artist = " << item.get_artist() << L"\n";
        }
        else if (m_target & std::to_underlying(Target::ALBUM))
        {
            modified_name = item.get_album();
            std::wcout << L"Album = " << item.get_album() << L"\n";
        }

        if (!FileNameValidator::try_fixing_name(modified_name))
        {
            continue;
        }

        std::filesystem::path new_path {
            file_path.parent_path()
            / (modified_name + file_path.extension().wstring())};

        if (m_verbose)
        {
            std::wcout << file_path << L" => " << new_path << L"\n";
        }

        if (m_do_modify
            && m_overwrite_prompt.is_overwriting(file_path, new_path))
        {
            // Try renaming the item and report if an error occurs.
            std::error_code err_code {};
            std::filesystem::rename(file_path, new_path, err_code);
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
 * @brief Construct a new MP3 Batch File Renamer object.
 *
 * @param paths Paths to process.
 * @param options Options to use for processing.
 */
MP3BatchFileRenamer::MP3BatchFileRenamer(
    const std::vector<std::string_view>& paths,
    const std::vector<std::string_view>& options)
    : m_paths {paths}, m_options {options}
{
    // m_collect_items(paths, options);
    m_process_options();
}

void MP3BatchFileRenamer::run()
{
    for (const auto& directory_path : m_paths)
    {
        m_process_items(directory_path);
    }
}

} // namespace Marvin
