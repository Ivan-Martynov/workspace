#include "StringAppendCommand.hpp"
#include "StringHelper.hpp"

#include <iostream>
#include <chrono>
#include <format>

namespace Marvin
{

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

void StringAppendCommand::append_numbers(
    std::vector<std::filesystem::path>& items) const
{
    size_t n {items.size()};
    size_t number_width {0};
    do
    {
        n /= 10;
        ++number_width;
    } while (n);

    for (size_t i {0}; i < items.size(); ++i)
    {
        std::filesystem::path& item_path {items[i]};

        const auto num_str {std::format(L"{:0{}}", (i + 1), number_width)};

        const auto name_stem {item_path.stem().wstring()};

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

void StringAppendCommand::append_timestamp(
    std::vector<std::filesystem::path>& items) const
{
    for (auto& item_path : items)
    {
        std::wstring time_point_format {};
        // std::chrono::system_clock::time_point time_point {};
        if (m_appendage == Appendage::CURRENT_TIME)
        {
            const std::chrono::zoned_time zoned {
                std::chrono::current_zone()->name(),
                std::chrono::system_clock::now()};

            time_point_format = std::format(L"{0:%Y_%m_%d_%H_%M_%S}",
                std::chrono::round<std::chrono::seconds>(
                    zoned.get_local_time()));
        }
        else
        {
            std::error_code err_code {};
            const std::filesystem::file_time_type time_stamp {
                std::filesystem::last_write_time(item_path, err_code)};

            if (err_code)
            {
                std::wcerr << L"what(): "
                           << StringHelper::string_to_wstring(
                                  err_code.message())
                           << L"\n";
                continue;
            }

            time_point_format = std::format(L"{0:%Y_%m_%d_%H_%M_%S}",
                std::chrono::round<std::chrono::seconds>(time_stamp));
        }

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

void StringAppendCommand::modify(
    std::vector<std::filesystem::path>& items, const size_t) const
{
    if ((m_method == Method::NONE) || (m_appendage == Appendage::NONE))
    {
        return;
    }

    if (m_appendage == Appendage::NUMBER)
    {
        append_numbers(items);
    }
    else
    {
        append_timestamp(items);
    }
}

} // namespace Marvin
