#ifndef _H_FILE_NAME_VALIDATOR_H_
#define _H_FILE_NAME_VALIDATOR_H_

#include <string>

namespace Marvin
{

class FileNameValidator
{
  private:
    FileNameValidator() = delete;

  public:
    static bool is_valid(const std::wstring_view&);
    static void replace_invalid_characters(
        std::wstring&, const std::wstring& = L"");
};

} // namespace Marvin

#endif // _H_FILE_NAME_VALIDATOR_H_
