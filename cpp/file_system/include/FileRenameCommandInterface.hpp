#ifndef _H_FILE_RENAME_COMMAND_INTERFACE_H_
#define _H_FILE_RENAME_COMMAND_INTERFACE_H_

#include <vector>
#include <filesystem>

namespace Marvin
{

class FileRenameCommandInterface
{
  public:
    virtual ~FileRenameCommandInterface() = default;

    virtual void modify(std::filesystem::path&) const = 0;

    virtual void modify(std::vector<std::filesystem::path>& items) const
    {
        for (auto& path : items)
        {
            modify(path);
        }
    }
};

} // namespace Marvin

#endif // _H_FILE_RENAME_COMMAND_INTERFACE_H_
