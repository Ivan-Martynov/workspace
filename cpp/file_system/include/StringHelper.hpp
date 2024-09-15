#ifndef _H_STRING_HELPER_H_
#define _H_STRING_HELPER_H_

#include <string>

namespace Marvin
{

class StringHelper
{
  private:
    StringHelper() = delete;

  public:
    static std::wstring string_to_wstring(const std::string&);
};

} // namespace Marvin

#endif // _H_STRING_HELPER_H_
