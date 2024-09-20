#ifndef _H_MP3_REPLACE_COMMAND_H_
#define _H_MP3_REPLACE_COMMAND_H_

#include "FileRenameCommandBase.h"

namespace Marvin
{

class MP3ReplaceCommand : public FileRenameCommandBase
{
  private:
    // String to subsitute with.
    std::wstring m_replacement;
  public:
    explicit MP3ReplaceCommand(std::wstring_view& replacement);

    ~MP3ReplaceCommand() = default;

    explicit MP3ReplaceCommand(const MP3ReplaceCommand& other) = default;
    MP3ReplaceCommand& operator=(const MP3ReplaceCommand& other) = default;

    void modify(std::filesystem::path&, const size_t) const override;
};

} // namespace Marvin

#endif // _H_MP3_REPLACE_COMMAND_H_
