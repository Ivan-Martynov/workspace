#ifndef _H_FILE_RENAME_COMMAND_INTERFACE_H_
#define _H_FILE_RENAME_COMMAND_INTERFACE_H_

#include <filesystem>

namespace Marvin
{

class FileRenameCommandInterface
{
  public:
    virtual ~FileRenameCommandInterface() = default;

    virtual void modify(std::filesystem::path&) const = 0;
};

} // namespace Marvin

#endif // _H_FILE_RENAME_COMMAND_INTERFACE_H_
