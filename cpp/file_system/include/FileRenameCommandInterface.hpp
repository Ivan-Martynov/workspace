#ifndef _H_FILE_RENAME_COMMAND_INTERFACE_H_
#define _H_FILE_RENAME_COMMAND_INTERFACE_H_

#include <vector>
#include <utility>
#include <filesystem>

namespace Marvin
{

class FileRenameCommandInterface
{
  protected:
  /**
   * @brief Get the filename object.
   * 
   * @param path Path.
   * @param extension_flag 
   * @return std::wstring 
   */
    std::wstring get_filename(
        std::filesystem::path& path, const size_t target_flag) const
    {
        std::wstring result {L""};

        if (target_flag & std::to_underlying(Target::STEM))
        {
            result += path.stem().wstring();
        }

        if (target_flag &  std::to_underlying(Target::EXTENSION))
        {
            result += path.extension().wstring();
        }

        return result;
    }

    void modify_filename(std::filesystem::path& path,
        const std::wstring& modified_part, const size_t target_flag) const
    {
        std::wstring replacement {L""};


        if (target_flag &  std::to_underlying(Target::STEM))
        {
            replacement += modified_part;
        }

        if (target_flag &  std::to_underlying(Target::EXTENSION))
        {
            replacement += modified_part;
        }

        path.replace_filename(modified_part);
    }

    virtual void modify(std::filesystem::path&, const size_t) const = 0;

  public:
    enum class Target : size_t
    {
        STEM,
        EXTENSION,
    };

    virtual ~FileRenameCommandInterface() = default;

    virtual void modify(std::vector<std::filesystem::path>& items,
        const size_t target_flag = std::to_underlying(Target::STEM)) const
    {
        for (auto& path : items)
        {
            modify(path, target_flag);
        }
    }
};

} // namespace Marvin

#endif // _H_FILE_RENAME_COMMAND_INTERFACE_H_
