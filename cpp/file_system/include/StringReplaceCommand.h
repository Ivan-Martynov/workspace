#ifndef _H_FILE_REPLACE_COMMAND_H_
#define _H_FILE_REPLACE_COMMAND_H_

#include "FileRenameCommandBase.h"

#include <regex>
#include <string>

namespace Marvin
{

/**
 * @brief Replacing part(s) of item name.
 *
 */
class StringReplaceCommand : public FileRenameCommandBase
{
  private:
    // String to look for.
    std::wstring m_match;
    // String to subsitute with.
    std::wstring m_replacement;

  public:
    /**
     * @brief Construct a new String Replace Command:: String Replace Command
     * object
     *
     * @param match String to look for.
     * @param replacement String to subsitute with.
     */
    explicit StringReplaceCommand(
        const std::wstring_view&, const std::wstring_view&);

    ~StringReplaceCommand() = default;

    explicit StringReplaceCommand(const StringReplaceCommand& other) = default;
    StringReplaceCommand& operator=(const StringReplaceCommand& other)
        = default;

    /**
     * @brief Modify path using target flag.
     *
     * @param path Path to an item.
     * @param target_flag Flag to determine which part of the item to modify.
     */
    void modify(std::filesystem::path&, const size_t) const override;
};

} // namespace Marvin

#endif // _H_FILE_REPLACE_COMMAND_H_
