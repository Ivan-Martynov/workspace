#include "StringAppendCommand.h"
#include "StringHelper.h"

#include <iostream>
#include <chrono>
#include <format>

namespace Marvin
{

/**
 * @brief Construct a new String Append Command:: String Append Command object
 *
 * @param method Append ('a') or prepend ('p').
 * @param appendage Number ('n'), timestamp ('t') or current time ('c').
 */
StringAppendCommand::StringAppendCommand(
    const char method, const char appendage)
{
    switch (method)
    {
        case 'a':
            m_method = Method::APPEND;
            break;
        case 'p':
            m_method = Method::PREPEND;
            break;
        default:
            break;
    }

    switch (appendage)
    {
        case 'n':
            m_appendage = Appendage::NUMBER;
            break;
        case 't':
            m_appendage = Appendage::TIMESTAMP;
            break;
        case 'c':
            m_appendage = Appendage::CURRENT_TIME;
            break;
        default:
            break;
    }
}

/**
 * @brief Append or prepend numbers consecutively to item name.
 *
 * @param items Items to process.
 */
void StringAppendCommand::m_append_numbers(
    std::span<std::pair<std::wstring, std::filesystem::path>> items) const
{
    // Calculate how many digits the number has in order to prepend with zeros
    // for proper sorting in different file systems.
    size_t n {items.size()};
    size_t number_width {0};
    do
    {
        n /= 10;
        ++number_width;
    } while (n);

    for (size_t i {0}; i < items.size(); ++i)
    {
        std::filesystem::path& item_path {items[i].second};

        // Construct a string objectd to append.
        const auto num_str {std::format(L"{:0{}}", (i + 1), number_width)};

        const auto name_stem {item_path.stem().wstring()};

        // Construct the modified item name.
        auto file_name {m_method == Method::APPEND
                            ? num_str + L'_' + name_stem
                            : name_stem + L'_' + num_str};

        if (item_path.has_extension())
        {
            file_name += item_path.extension().wstring();
        }

        item_path.replace_filename(file_name);
    }
}

/**
 * @brief Append or prepend timestamp to item name.
 *
 * @param items Items to process.
 */
void StringAppendCommand::m_append_timestamp(
    std::span<std::pair<std::wstring, std::filesystem::path>> items) const
{
    for (auto& pair : items)
    {
        std::filesystem::path& item_path {pair.second};

        std::wstring time_point_format {};
        if (m_appendage == Appendage::CURRENT_TIME)
        {
            // Get the time for the current zone.
            const std::chrono::zoned_time zoned {
                std::chrono::current_zone()->name(),
                std::chrono::system_clock::now()};

            // Format the string by separating date components with underscores.
            time_point_format = std::format(L"{0:%Y_%m_%d_%H_%M_%S}",
                std::chrono::round<std::chrono::seconds>(
                    zoned.get_local_time()));
        }
        else
        {
            std::error_code err_code {};
            const std::filesystem::file_time_type time_stamp {
                std::filesystem::last_write_time(item_path, err_code)};

            // Read the timestamp of the item and report if an error occurs.
            if (err_code)
            {
                std::wcerr << L"what(): "
                           << StringHelper::string_to_wstring(
                                  err_code.message())
                           << L"\n";
                continue;
            }

            // Format the string by separating date components with underscores.
            time_point_format = std::format(L"{0:%Y_%m_%d_%H_%M_%S}",
                std::chrono::round<std::chrono::seconds>(time_stamp));
        }

        // Construct the modified item name.
        const auto name_stem {item_path.stem().wstring()};
        auto file_name {m_method == Method::APPEND
                            ? time_point_format + L'_' + name_stem
                            : name_stem + L'_' + time_point_format};

        if (item_path.has_extension())
        {
            file_name += item_path.extension().wstring();
        }

        item_path.replace_filename(file_name);
    }
}

/**
 * @brief Modify path using target flag.
 *
 * @param path Path to an item.
 * @param target_flag Flag to determine which part of the item to modify.
 */
void StringAppendCommand::modify(
    std::span<std::pair<std::wstring, std::filesystem::path>> items,
    const size_t) const
{
    if ((m_method == Method::NONE) || (m_appendage == Appendage::NONE))
    {
        return;
    }

    if (m_appendage == Appendage::NUMBER)
    {
        m_append_numbers(items);
    }
    else
    {
        m_append_timestamp(items);
    }
}

} // namespace Marvin
