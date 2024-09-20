#include "StringReplaceCommand.h"

namespace Marvin
{

/**
 * @brief Construct a new String Replace Command:: String Replace Command object
 *
 * @param match String to look for.
 * @param replacement String to subsitute with.
 */
StringReplaceCommand::StringReplaceCommand(
    const std::wstring_view& match, const std::wstring_view& replacement)
    : m_match {match}, m_replacement {replacement}
{
}

/**
 * @brief Modify path using target flag.
 *
 * @param path Path to an item.
 * @param target_flag Flag to determine which part of the item to modify.
 */
void StringReplaceCommand::modify(
    std::filesystem::path& path, const size_t target_flag) const
{
    std::wstring file_name {get_filepart(path, target_flag)};

    // Replace using regex.
    file_name
        = std::regex_replace(file_name, std::wregex(m_match), m_replacement);

    modify_filename(path, file_name, target_flag);
}

} // namespace Marvin
