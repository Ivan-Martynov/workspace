#ifndef _H_STRING_HELPER_H_
#define _H_STRING_HELPER_H_

#include <string>

namespace Marvin
{

/**
 * @brief Helper class for string objects.
 */
class StringHelper
{
  private:
    StringHelper() = delete;

  public:
    /**
     * @brief Convert string object into wide string.
     *
     * @param src String to convert.
     * @return std::wstring Wide character string.
     */
    static std::wstring string_to_wstring(const std::string&);
    static std::string wstring_to_string(const std::wstring&);

    static std::string iso_8859_1_to_string(std::string& input);

    static bool equal_ignore_case(
        const std::string_view&, const std::string_view&);
    static bool equal_ignore_case(
        const std::wstring_view&, const std::wstring_view&);

};

} // namespace Marvin

#endif // _H_STRING_HELPER_H_
