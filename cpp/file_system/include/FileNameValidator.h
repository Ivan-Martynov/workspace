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
    /**
     * @brief Check whether the name contains at least one invalid character.
     *
     * @param[in] file_name Name to check.
     * @return true The name contains at least one invalid character.
     * @return false otherwise.
     */
    static bool contains_invalid_character(const std::wstring_view&);

    /**
     * @brief Check if the name is valid.
     *
     * @param file_path Name to check.
     * @return true if name is valid, false otherwise.
     */
    static bool is_valid(const std::wstring_view&);

    /**
     * @brief Check if the file name ends with a period (dot) or space.
     *
     * @param[in] file_name Name to check.
     * @return true if the file name ends with a period (dot) or space.
     * @return false otherwise.
     */
    static bool ends_with_space_or_period(const std::wstring_view& file_name);

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
     * @brief Try fixing file name by replacing invalid characters and trimming
     * if needed.
     *
     * @param[in] file_name Path name.
     * @param[in] replacement Replacement for each invalid character.
     * @return true If name is modified into a valid one, false otherwise.
     */
    static bool try_fixing_name(std::wstring&, const std::wstring& = L"_");
};

} // namespace Marvin

#endif // _H_FILE_NAME_VALIDATOR_H_
