#ifndef _H_STRING_REMOVAL_COMMAND_H_
#define _H_STRING_REMOVAL_COMMAND_H_

#include "FileRenameCommandInterface.hpp"

#include <string>
#include <regex>
#include <iostream>

namespace Marvin
{

class StringRemovalCommand : public FileRenameCommandInterface
{
  private:
    std::wstring m_match;

  public:
    explicit StringRemovalCommand(
        const std::wstring& match, const std::wstring& replacement)
        : m_match {match}
    {
    }

    ~StringRemovalCommand() = default;

    explicit StringRemovalCommand(const StringRemovalCommand& other) = default;
    StringRemovalCommand& operator=(const StringRemovalCommand& other)
        = default;

    void modify(std::filesystem::path& path) const override
    {
        std::wcout << L"Path " << path << L" => ";
        path.replace_filename(std::regex_replace(
            path.filename().wstring(), std::wregex(m_match), L""));
        std::wcout << path << L"\n";
    }
};

} // namespace Marvin

#endif // _H_STRING_REMOVAL_COMMAND_H_
