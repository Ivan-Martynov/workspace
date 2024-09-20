#include "MP3ReplaceCommand.h"

namespace Marvin
{

MP3ReplaceCommand::MP3ReplaceCommand(std::wstring_view& replacement)
    : m_replacement {replacement}
{
}

void MP3ReplaceCommand::modify(
    std::filesystem::path& path, const size_t target_flag) const
{
    modify_filename(path, m_replacement, target_flag);
}

} // namespace Marvin
