#include "FileOrganizer.h"
#include "StringHelper.h"

#include <iostream>
#include <chrono>
#include <format>
#include <fstream>
#include <regex>
#include <execution>
#include <thread>

namespace Marvin
{

FileOrganizer::FileOrganizer(const std::vector<std::string_view>& paths,
    const std::vector<std::string_view>& options)
    : m_paths {paths}, m_options {options}
{
    for (const auto& option : options) {
        if ((option == "-h") || (option == "-help")) {
            m_do_show_help = true;
        } else if ((option == "-v") || (option == "-verbose")) {
            m_verbose = true;
        } else if ((option == "-dow") || (option == "-do-overwrite")) {
            m_overwrite_prompt.set_mode(FileOverwritePrompt::Mode::YES_TO_ALL);
        } else if ((option == "-dowu") || (option == "-do-overwrite-update")) {
            m_overwrite_prompt.set_mode(
                FileOverwritePrompt::Mode::YES_TO_ALL_UPDATE);
        } else if ((option == "-dnow") || (option == "-do-not-overwrite")) {
            m_overwrite_prompt.set_mode(FileOverwritePrompt::Mode::NO_TO_ALL);
        } else if ((option == "-dc") || (option == "-do-copy")) {
            m_do_copy = true;
        } else if ((option == "-dm") || (option == "-do-move")) {
            m_do_move = true;
        } else if ((option == "-y") || (option == "-year")) {
            m_folder_target |= static_cast<size_t>(FolderTarget::YEAR);
        } else if ((option == "-yo") || (option == "-year-only")) {
            m_folder_target = static_cast<size_t>(FolderTarget::YEAR);
        } else if ((option == "-m") || (option == "-month")) {
            m_folder_target |= static_cast<size_t>(FolderTarget::MONTH);
        } else if ((option == "-mo") || (option == "-month-only")) {
            m_folder_target = static_cast<size_t>(FolderTarget::MONTH);
        } else if ((option == "-d") || (option == "-day")) {
            m_folder_target |= static_cast<size_t>(FolderTarget::DAY);
        } else if ((option == "-do") || (option == "-day-only")) {
            m_folder_target = static_cast<size_t>(FolderTarget::DAY);
        } else if ((option == "-H") || (option == "-hour")) {
            m_folder_target |= static_cast<size_t>(FolderTarget::HOUR);
        } else if ((option == "-ho") || (option == "-hour-only")) {
            m_folder_target = static_cast<size_t>(FolderTarget::HOUR);
        } else if ((option == "-M") || (option == "-minute")) {
            m_folder_target |= static_cast<size_t>(FolderTarget::MINUTE);
        } else if ((option == "-Mo") || (option == "-minute-only")) {
            m_folder_target = static_cast<size_t>(FolderTarget::MINUTE);
        } else if ((option == "-s") || (option == "-second")) {
            m_folder_target |= static_cast<size_t>(FolderTarget::SECOND);
        } else if ((option == "-so") || (option == "-second-only")) {
            m_folder_target = static_cast<size_t>(FolderTarget::SECOND);
        }
    }
}

std::filesystem::path FileOrganizer::extract_date_from_name(
    const std::filesystem::path& current_path)
{
    const auto r {std::wregex {L"(\\d{8})"}};
    std::wsmatch match {};
    const auto name {current_path.filename().wstring()};
    if (std::regex_search(name, match, r)) {
        const auto date_str {match[0].str()};
        std::filesystem::path target_path {current_path.parent_path()};
        if (m_folder_target & static_cast<size_t>(FolderTarget::YEAR)) {
            target_path /= date_str.substr(0, 4);
        }
        if (m_folder_target & static_cast<size_t>(FolderTarget::MONTH)) {
            target_path /= date_str.substr(4, 2) + L'_' + std::format(
                L"{0:%B}", std::chrono::month {
                               (date_str[4] == '0' ? 0 : 10)
                               + static_cast<unsigned int>(date_str[5] - '0')});
        }
        if (m_folder_target & static_cast<size_t>(FolderTarget::DAY)) {
            target_path /= date_str.substr(6);
        }
        return target_path;
    }
    return "";
}

std::filesystem::path FileOrganizer::build_target_path(
    const std::filesystem::directory_entry& item)
{
    const auto time_stamp {
        std::chrono::round<std::chrono::seconds>(item.last_write_time())};

    const auto current_path {item.path()};
#if 0
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
#else
    std::filesystem::path target_path {extract_date_from_name(current_path)};
#endif
    return target_path / current_path.filename();
}

std::mutex mutex {};
void FileOrganizer::actual_copy_or_move(
    const std::filesystem::path& current_path,
    const std::filesystem::path& target_path)
{
    std::error_code err_code {};

    {
        std::lock_guard {mutex};
        std::filesystem::create_directories(
            target_path.parent_path(), err_code);
    }

    if (err_code) {
        std::wcerr << L"what(): "
                   << StringHelper::string_to_wstring(err_code.message())
                   << L"\n";
        return;
    }

    err_code.clear();
    if (m_do_move) {
        if (m_verbose) {
            std::wcout << L"Moving file " << current_path << " to "
                       << target_path << L" ... ";
        }
        std::filesystem::rename(current_path, target_path, err_code);
    } else {
        if (m_verbose) {
            std::wcout << L"Copying file " << current_path << " to "
                       << target_path << L" ... ";
        }
        auto copy_lambda {[&current_path, &target_path]() {
            std::ifstream input_stream {current_path, std::ios::binary};
            std::ofstream output_stream {target_path, std::ios::binary};

            if (!input_stream.good() || !output_stream.good()) {
                std::wcerr << L"Couldn't open files for copying: "
                           << current_path.wstring() << " => "
                           << target_path.wstring() << "\n";
                return;
            }

            output_stream << input_stream.rdbuf();
        }};
#if 0
        //std::filesystem::copy_file(current_path, target_path, err_code);
        std::filesystem::copy(current_path, target_path,
             std::filesystem::copy_options::update_existing, err_code);
#else
        copy_lambda();
#endif
    }

    if (err_code) {
        std::wcerr << L"what(): "
                   << StringHelper::string_to_wstring(err_code.message())
                   << L"\n";
    }

    if (m_verbose) {
        std::wcout << L"Done.\n";
    }
}

void FileOrganizer::process_item(const std::filesystem::directory_entry& item)
{
    if (!item.is_regular_file()) {
        return;
    }

    const auto current_path {item.path()};
    const std::filesystem::path target_path {build_target_path(item)};
    if (m_verbose) {
        std::wcout << current_path << L" => " << target_path << L"\n";
    }

    if (m_do_copy || m_do_move) {
        {
            std::lock_guard {mutex};
            if (!m_overwrite_prompt.is_overwriting(current_path, target_path)) {
                return;
            }
        }

        actual_copy_or_move(current_path, target_path);
    }
}

void FileOrganizer::process_path(const std::string_view& path)
{
    if (!std::filesystem::is_directory(path)) {
        return;
    }

    std::error_code err_code {};
    auto it {std::filesystem::directory_iterator {path,
        std::filesystem::directory_options::skip_permission_denied, err_code}};

    if (err_code) {
        std::wcerr << L"what(): "
                   << StringHelper::string_to_wstring(err_code.message())
                   << L"\n";
        return;
    }

    std::vector<std::filesystem::directory_entry> v {};
    v.assign(it, std::filesystem::directory_iterator {});

#if 0
    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {path}) {
        process_item(item);
    }
#else
    std::for_each(std::execution::par_unseq, v.cbegin(), v.cend(),
        [this](const auto& item) { this->process_item(item); });
#endif
}

void FileOrganizer::run()
{
    if (m_folder_target == static_cast<size_t>(FolderTarget::NONE)) {
        m_folder_target = static_cast<size_t>(FolderTarget::YEAR)
                          | static_cast<size_t>(FolderTarget::MONTH);
    }

    for (const auto& path : m_paths) {
        process_path(path);
    }
}

} // namespace Marvin
