#ifndef _H_STRING_APPEND_COMMAND_H_
#define _H_STRING_APPEND_COMMAND_H_

#include "FileRenameCommandBase.h"

namespace Marvin
{

/**
 * @brief Class to append or prepend text to item name.
 */
class StringAppendCommand : public FileRenameCommandBase
{
  private:
    // Appending or prepending.
    enum class Method
    {
        NONE,
        APPEND,
        PREPEND,
    };

    // What to append.
    enum class Appendage
    {
        NONE,
        NUMBER,
        TIMESTAMP,
        CURRENT_TIME,
    };

    Method m_method {Method::NONE};
    Appendage m_appendage {Appendage::NONE};

    /**
     * @brief Append or prepend numbers consecutively to item name.
     *
     * @param items Items to process.
     */
    void m_append_numbers(
        std::span<std::pair<std::wstring, std::filesystem::path>> items)
        const;
    /**
     * @brief Append or prepend numbers consecutively to item name.
     *
     * @param items Items to process.
     */
    void m_append_timestamp(
        std::span<std::pair<std::wstring, std::filesystem::path>> items)
        const;

  public:
    /**
     * @brief Construct a new String Append Command:: String Append Command
     * object
     *
     * @param method Append ('a') or prepend ('p').
     * @param appendage Number ('n'), timestamp ('t') or current time ('c').
     */
    explicit StringAppendCommand(const char, const char);

    ~StringAppendCommand() = default;

    explicit StringAppendCommand(const StringAppendCommand& other) = default;
    StringAppendCommand& operator=(const StringAppendCommand& other) = default;

    /**
     * @brief Override the function to do nothing. Instead, items are processed
     * in the sorted order.
     */
    void modify(std::filesystem::path&, const size_t) const override {}

    /**
     * @brief Modify path using target flag.
     *
     * @param path Path to an item.
     * @param target_flag Flag to determine which part of the item to modify.
     */
    void modify(
        std::span<std::pair<std::wstring, std::filesystem::path>> items,
        const size_t) const override;
};

} // namespace Marvin

#endif // _H_STRING_APPEND_COMMAND_H_
