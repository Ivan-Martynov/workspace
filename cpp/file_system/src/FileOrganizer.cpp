#include "FileOrganizer.h"
#include "StringHelper.h"

#include <iostream>
#include <chrono>
#include <format>

namespace Marvin
{

FileOrganizer::FileOrganizer(const std::vector<std::string_view>& paths,
    const std::vector<std::string_view>& options)
    : m_paths {paths}, m_options {options}
{
    for (size_t i {0}; i < m_options.size(); ++i)
    {
        const auto& option {m_options[i]};
        if ((option == "-h") || (option == "-help"))
        {
            m_do_show_help = true;
        }
        else if ((option == "-v") || (option == "-verbose"))
        {
            m_verbose = true;
        }
        else if ((option == "-dow") || (option == "-do-overwrite"))
        {
            m_do_overwrite = Overwrite::YES;
        }
        else if ((option == "-dnow") || (option == "-do-not-overwrite"))
        {
            m_do_overwrite = Overwrite::NO;
        }
        else if ((option == "-dc") || (option == "-do-copy"))
        {
            m_do_copy = true;
        }
        else if ((option == "-dm") || (option == "-do-move"))
        {
            m_do_move = true;
        }
        else if ((option == "-y") || (option == "-year"))
        {
            m_folder_target |= static_cast<size_t>(FolderTarget::YEAR);
        }
        else if ((option == "-yo") || (option == "-year-only"))
        {
            m_folder_target = static_cast<size_t>(FolderTarget::YEAR);
        }
        else if ((option == "-m") || (option == "-month"))
        {
            m_folder_target |= static_cast<size_t>(FolderTarget::MONTH);
        }
        else if ((option == "-mo") || (option == "-month-only"))
        {
            m_folder_target = static_cast<size_t>(FolderTarget::MONTH);
        }
        else if ((option == "-d") || (option == "-day"))
        {
            m_folder_target |= static_cast<size_t>(FolderTarget::DAY);
        }
        else if ((option == "-do") || (option == "-day-only"))
        {
            m_folder_target = static_cast<size_t>(FolderTarget::DAY);
        }
        else if ((option == "-H") || (option == "-hour"))
        {
            m_folder_target |= static_cast<size_t>(FolderTarget::HOUR);
        }
        else if ((option == "-ho") || (option == "-hour-only"))
        {
            m_folder_target = static_cast<size_t>(FolderTarget::HOUR);
        }
        else if ((option == "-M") || (option == "-minute"))
        {
            m_folder_target |= static_cast<size_t>(FolderTarget::MINUTE);
        }
        else if ((option == "-Mo") || (option == "-minute-only"))
        {
            m_folder_target = static_cast<size_t>(FolderTarget::MINUTE);
        }
        else if ((option == "-s") || (option == "-second"))
        {
            m_folder_target |= static_cast<size_t>(FolderTarget::SECOND);
        }
        else if ((option == "-so") || (option == "-second-only"))
        {
            m_folder_target = static_cast<size_t>(FolderTarget::SECOND);
        }
    }
}

std::filesystem::path FileOrganizer::build_target_path(
    const std::filesystem::directory_entry& item)
{
    const auto time_stamp {
        std::chrono::round<std::chrono::seconds>(item.last_write_time())};

    const auto current_path {item.path()};
    std::filesystem::path target_path {current_path.parent_path()};
    if (m_folder_target & static_cast<size_t>(FolderTarget::YEAR))
    {
        target_path /= std::format(L"{0:%Y}", time_stamp);
    }
    if (m_folder_target & static_cast<size_t>(FolderTarget::MONTH))
    {
        target_path /= std::format(L"{0:%B}", time_stamp);
    }
    if (m_folder_target & static_cast<size_t>(FolderTarget::DAY))
    {
        target_path /= std::format(L"{0:%d}", time_stamp);
    }
    if (m_folder_target & static_cast<size_t>(FolderTarget::HOUR))
    {
        target_path /= std::format(L"{0:%H}", time_stamp);
    }
    if (m_folder_target & static_cast<size_t>(FolderTarget::MINUTE))
    {
        target_path /= std::format(L"{0:%M}", time_stamp);
    }
    if (m_folder_target & static_cast<size_t>(FolderTarget::SECOND))
    {
        target_path /= std::format(L"{0:%S}", time_stamp);
    }

    return target_path / current_path.filename();
}

void FileOrganizer::actual_copy_or_move(
    const std::filesystem::path& current_path,
    const std::filesystem::path& target_path)
{
    std::error_code err_code {};
    std::filesystem::create_directories(target_path.parent_path(), err_code);
    if (err_code)
    {
        std::wcerr << L"what(): "
                   << StringHelper::string_to_wstring(err_code.message())
                   << L"\n";
        return;
    }

    err_code.clear();
    if (m_do_move)
    {
        if (m_verbose)
        {
            std::wcout << L"Moving file " << current_path << " to "
                       << target_path << L" ... ";
        }
        std::filesystem::rename(current_path, target_path, err_code);
    }
    else
    {
        if (m_verbose)
        {
            std::wcout << L"Copying file " << current_path << " to "
                       << target_path << L" ... ";
        }
        std::filesystem::copy_file(current_path, target_path, err_code);
    }

    if (m_verbose)
    {
        std::wcout << L"Done.\n";
    }

    if (err_code)
    {
        std::wcerr << L"what(): "
                   << StringHelper::string_to_wstring(err_code.message())
                   << L"\n";
    }
}

void FileOrganizer::process_item(const std::filesystem::directory_entry& item)
{
    if (!item.is_regular_file())
    {
        return;
    }

    const auto current_path {item.path()};
    const std::filesystem::path target_path {build_target_path(item)};

    if (m_verbose)
    {
        std::wcout << current_path << L" => " << target_path << L"\n";
    }

    if (m_do_copy || m_do_move)
    {
        if (std::filesystem::exists(target_path))
        {
            if (m_do_overwrite != Overwrite::NO)
            {
                std::wcout << L"File already exists: " << target_path << L'\n';
            }

            bool do_overwrite {m_do_overwrite == Overwrite::YES};
            if (!do_overwrite && (m_do_overwrite != Overwrite::NO))
            {
                // Use letters to process the answer:
                // 'n' - do not overwrite the current item;
                // 'y' - do overwrite the current item;
                // 'a' - do overwrite existing items if needed;
                // 'o' - do not overwrite existing items.
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
                return;
            }
        }

        actual_copy_or_move(current_path, target_path);
    }
}

void FileOrganizer::process_path(const std::string_view& path)
{
    if (!std::filesystem::is_directory(path))
    {
        return;
    }

    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {path})
    {
        process_item(item);
    }
}

void FileOrganizer::run()
{
    if (m_folder_target == static_cast<size_t>(FolderTarget::NONE))
    {
        m_folder_target = static_cast<size_t>(FolderTarget::YEAR)
                          | static_cast<size_t>(FolderTarget::MONTH);
    }

    for (const auto& path : m_paths)
    {
        process_path(path);
    }
}

} // namespace Marvin
