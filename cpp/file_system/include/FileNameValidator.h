#ifndef _H_FILE_NAME_VALIDATOR_H_
#define _H_FILE_NAME_VALIDATOR_H_

#include <string>

namespace Marvin
{

/**
 * @brief Tool to check if the item name is valid.
 */
class FileNameValidator
{
  private:
    // Static class.
    FileNameValidator() = delete;

  public:
    static bool contains_invalid_character(const std::wstring_view&);

    /**
     * @brief Check if the name is valid.
     *
     * @param file_path Name to check.
     * @return true if name is valid, false otherwise.
     */
    static bool is_valid(const std::wstring_view&);

    /**
     * @brief Check if file name is too long.
     *
     * @param file_path Item name to check.
     * @return true If name is too long, false otherwise.
     */
    static bool filename_too_long(const std::wstring_view&);

    /**
     * @brief Check if directory name is too long.
     *
     * @param file_path Item name to check.
     * @return true If name is too long, false otherwise.
     */
    static bool directory_name_too_long(const std::wstring_view&);

    /**
     * @brief Replace invalid characters in the name name.
     *
     * @param file_name Path name.
     * @param replacement Replacement for each invalid character.
     */
    static void replace_invalid_characters(
        std::wstring&, const std::wstring& = L"_");
};

} // namespace Marvin

#endif // _H_FILE_NAME_VALIDATOR_H_
