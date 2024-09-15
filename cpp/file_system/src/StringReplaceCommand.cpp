#include "StringReplaceCommand.hpp"

namespace Marvin
{

StringReplaceCommand::StringReplaceCommand(
    const std::wstring_view& match, const std::wstring_view& replacement)
    : m_match {match}, m_replacement {replacement}
{
}

void StringReplaceCommand::modify(
    std::filesystem::path& path, const size_t target_flag) const
{
    std::wstring file_name {get_filename(path, target_flag)};

    file_name
        = std::regex_replace(file_name, std::wregex(m_match), m_replacement);

    modify_filename(path, file_name, target_flag);
}

} // namespace Marvin
