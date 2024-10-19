#ifndef _H_FILE_RENAME_COMMAND_BASE_H_
#define _H_FILE_RENAME_COMMAND_BASE_H_

#include <filesystem>
#include <utility>
#include <vector>
#include <span>

namespace Marvin
{

/**
 * @brief Base class to rename file paths. The class is to be a parent class for
 * other renaming classes.
 */
class FileRenameCommandBase
{
  protected:
    /**
     * @brief Get the filename object.
     *
     * @param path Path to an item.
     * @param extension_flag Flag to determine which part of the item is used.
     * @return std::wstring File part to be modified.
     */
    std::wstring get_filepart(std::filesystem::path&, const size_t) const;

    /**
     * @brief Modify the name of the path according to the flag.
     *
     * @param path Path to an item.
     * @param modified_part Part of the path which has been modified.
     * @param target_flag Flag determining which part was modified.
     */
    void modify_filename(
        std::filesystem::path&, const std::wstring&, const size_t) const;

    /**
     * @brief Modify path using target flag.
     *
     * @param path Path to an item.
     * @param target_flag Flag to determine which part of the item to modify.
     */
    virtual void modify(std::filesystem::path&, const size_t) const = 0;

  public:
    /**
     * @brief Enumeration for two possible targets: stem and extension.
     */
    enum class Target : size_t
    {
        STEM = 1,
        EXTENSION = 2,
    };

    virtual ~FileRenameCommandBase() = default;

    /**
     * @brief Modify items using target flag.
     *
     * @param items Items to process.
     * @param target_flag Flag to determine which part of an item to modify.
     */
    virtual void modify(
        std::span<std::pair<std::wstring, std::filesystem::path>> items,
        const size_t = std::to_underlying(Target::STEM)) const;
};

} // namespace Marvin

#endif // _H_FILE_RENAME_COMMAND_BASE_H_
