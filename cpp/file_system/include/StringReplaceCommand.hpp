#ifndef _H_FILE_REPLACE_COMMAND_H_
#define _H_FILE_REPLACE_COMMAND_H_

#include "FileRenameCommandInterface.hpp"

#include <string>
#include <regex>

namespace Marvin
{

class StringReplaceCommand : public FileRenameCommandInterface
{
  private:
    std::wstring m_match;
    std::wstring m_replacement;

  public:
    explicit StringReplaceCommand(
        const std::wstring_view&, const std::wstring_view&);

    ~StringReplaceCommand() = default;

    explicit StringReplaceCommand(const StringReplaceCommand& other) = default;
    StringReplaceCommand& operator=(const StringReplaceCommand& other)
        = default;

    void modify(std::filesystem::path&, const size_t) const override;
};

} // namespace Marvin

#endif // _H_FILE_REPLACE_COMMAND_H_
