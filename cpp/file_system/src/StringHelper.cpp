#include "StringHelper.h"

#include <cwctype>

namespace Marvin
{

/**
 * @brief Convert string object into wide string.
 *
 * @param src String to convert.
 * @return std::wstring Wide character string.
 */
std::wstring StringHelper::string_to_wstring(const std::string& src)
{
    std::wstring result(src.length() + 1, L'\0');
    std::mbstowcs(result.data(), src.c_str(), result.length());

    return result;
}

std::string StringHelper::wstring_to_string(const std::wstring& src)
{
    std::string result(src.length() + 1, L'\0');
    std::wcstombs(result.data(), src.c_str(), result.length());

    return result;
}

/**
 * @brief Compare two string case insensitive.
 * 
 * @param[in] lhs First string.
 * @param[in] rhs Second string.
 * @return true if strings are equal ignoring the case, false otherwise.
 */
bool StringHelper::equal_ignore_case(
    const std::string_view& lhs, const std::string_view& rhs)
{
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
        [](auto a, auto b) { return std::tolower(a) == std::tolower(b); });
}

/**
 * @brief Compare two wide string case insensitive.
 * 
 * @param[in] lhs First string.
 * @param[in] rhs Second string.
 * @return true if strings are equal ignoring the case, false otherwise.
 */
bool StringHelper::equal_ignore_case(
    const std::wstring_view& lhs, const std::wstring_view& rhs)
{
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
        [](auto a, auto b) { return std::towlower(a) == std::towlower(b); });
}

std::string StringHelper::iso_8859_1_to_string(std::string& input)
{
    const size_t n {input.length()};
    std::string output {};
    output.reserve(2 * n + 1);

#if 1
    int j {-1};

    for (size_t i {0}; i < n; ++i)
    {
        const unsigned char c {static_cast<unsigned char>(input[i])};

        if (c < 0x80)
        {
            output[++j] = c;
        }
        else
        {
            // first byte is 0xc2 for 0x80-0xbf, 0xc3 for 0xc0-0xff
            // (the condition in () evaluates to true / 1)
            output[++j] = 0xc2 + (c > 0xbf);

            // second byte is the lower six bits of the input byte
            // with the highest bit set (and, implicitly, the second-
            // highest bit unset)
            output[++j] = (c & 0x3f) + 0x80;
        }
    }

    return output;
#endif
}

} // namespace Marvin
