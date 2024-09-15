#include "StringHelper.hpp"

namespace Marvin
{

std::wstring StringHelper::string_to_wstring(const std::string& src)
{
    std::wstring result(src.length() + 1, L'\0');
    std::mbstowcs(result.data(), src.c_str(), result.length());

    return result;
}

} // namespace Marvin

