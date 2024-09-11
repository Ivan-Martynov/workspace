#ifndef _H_CASE_MODIFY_COMMAND_H_
#define _H_CASE_MODIFY_COMMAND_H_

#include "FileRenameCommandInterface.hpp"

#include <algorithm>
#include <iostream>

namespace Marvin
{

class CaseModifyCommand : public FileRenameCommandInterface
{
  private:
    char m_method;

  public:
    CaseModifyCommand(const char c) : m_method {c} {}

    ~CaseModifyCommand() = default;

    explicit CaseModifyCommand(const CaseModifyCommand& other) = default;
    CaseModifyCommand& operator=(const CaseModifyCommand& other) = default;

    void modify(std::filesystem::path& path) const override
    {
        std::wcout << L"CaseModidyCommand: " << path << L" => ";
        std::wcout << L"Stem: " << path.stem() << L" => ";
        auto s = path.filename().wstring();
        if (m_method == 'u') // uppecase
        {
            std::transform(s.begin(), s.end(), s.begin(), ::towupper);
        }
        else if (m_method == 'l') // lowercase
        {
            std::transform(s.begin(), s.end(), s.begin(), ::towlower);
        }
        else if (m_method == 'c') // camelcase
        {
            bool expect_first_letter {true};
            std::transform(s.begin(), s.end(), s.begin(),
                [&expect_first_letter](wchar_t c)
                {
                    if (std::iswalpha(c))
                    {
                        if (expect_first_letter)
                        {
                            expect_first_letter = false;
                            return std::towupper(c);
                        }
                        else
                        {
                            return std::towlower(c);
                        }
                    }
                    else
                    {
                        expect_first_letter = true;
                        return static_cast<wint_t>(c);
                    }
                });
        }
        path.replace_filename(s);
        std::wcout << path << L"\n";
    }
};

} // namespace Marvin

#endif // _H_CASE_MODIFY_COMMAND_H_
