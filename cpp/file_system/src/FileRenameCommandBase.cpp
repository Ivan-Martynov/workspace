#include "FileRenameCommandBase.h"

namespace Marvin
{
/**
 * @brief Get the filename object.
 *
 * @param path Path to an item.
 * @param extension_flag Flag to determine which part of the item is used.
 * @return std::wstring File part to be modified.
 */
std::wstring FileRenameCommandBase::get_filepart(
    std::filesystem::path& path, const size_t target_flag) const
{
    if (target_flag & std::to_underlying(Target::STEM))
    {
        return target_flag & std::to_underlying(Target::EXTENSION)
                   ? path.filename().wstring()
                   : path.stem().wstring();
    }
    else // Only extension is modified.
    {
        return path.extension().wstring();
    }
}

/**
 * @brief Modify the name of the path according to the flag.
 *
 * @param path Path to an item.
 * @param modified_part Part of the path which has been modified.
 * @param target_flag Flag determining which part was modified.
 */
void FileRenameCommandBase::modify_filename(std::filesystem::path& path,
    const std::wstring& modified_part, const size_t target_flag) const
{
    if (target_flag & std::to_underlying(Target::STEM))
    {
        path.replace_filename(
            target_flag & std::to_underlying(Target::EXTENSION)
                ? modified_part
                : modified_part + path.extension().wstring());
    }
    else // Only extension is modified.
    {
        path.replace_filename(path.stem().wstring() + modified_part);
    }
}

/**
 * @brief Modify items using target flag.
 *
 * @param items Items to process.
 * @param target_flag Flag to determine which part of an item to modify.
 */
void FileRenameCommandBase::modify(
    std::span<std::pair<std::wstring, std::filesystem::path>> items,
    const size_t target_flag) const
{
    for (auto& path : items)
    {
        modify(path.second, target_flag);
    }
}

} // namespace Marvin
