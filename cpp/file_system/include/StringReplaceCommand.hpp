#ifndef _H_FILE_REPLACE_COMMAND_H_
#define _H_FILE_REPLACE_COMMAND_H_

#include "FileRenameCommandInterface.hpp"

#include <string>
#include <regex>
#include <iostream>

namespace Marvin
{

class StringReplaceCommand : public FileRenameCommandInterface
{
  private:
    std::wstring m_match;
    std::wstring m_replacement;

  public:
    explicit StringReplaceCommand(
        const std::wstring& match, const std::wstring& replacement)
        : m_match {match}, m_replacement {replacement}
    {
    }

    ~StringReplaceCommand() = default;

    explicit StringReplaceCommand(const StringReplaceCommand& other) = default;
    StringReplaceCommand& operator=(const StringReplaceCommand& other) = default;

    void modify(std::filesystem::path& path) const override
    {
        std::wcout << L"StringReplaceCommand: " << path.filename() << L" => ";
        path.replace_filename(std::regex_replace(
            path.filename().wstring(), std::wregex(m_match), m_replacement));
        std::wcout << path.filename() << L"\n";
    }
};

} // namespace Marvin

#endif // _H_FILE_REPLACE_COMMAND_H_
