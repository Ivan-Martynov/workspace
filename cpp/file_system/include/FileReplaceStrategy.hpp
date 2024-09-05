#ifndef _H_FILE_REPLACE_STRATEGY_H_
#define _H_FILE_REPLACE_STRATEGY_H_

#include "FileRenameStrategy.hpp"

#include <filesystem>

namespace Marvin
{

class FileReplaceStrategy : public FileRenameStrategy
{
  private:
    std::wstring m_match {L""};
    std::wstring m_replacement {L""};

  public:
    FileReplaceStrategy(const std::string_view& path,
        const std::vector<std::string_view>& options)
        : FileRenameStrategy(path, options)
    {
        for (size_t i {0}; i < options.size(); ++i)
        {
            const auto option {options[i]};
            if ((option == "-rur") || (option == "-replace-using-regex")
                || (option == "-rsw") || (option == "-replace-substring-with"))
            {
                if (++i < options.size())
                {
                    // A trick to convert string to wstring.
                    m_match = std::filesystem::path(options[i]).wstring();
                }

                if (++i < options.size())
                {
                    m_replacement = std::filesystem::path(options[i]).wstring();
                }
            }
            else if ((option == "-rww")
                     || (option == "-replace-whitespace-with"))
            {
                if (++i < options.size())
                {
                    m_replacement = std::filesystem::path(options[i]).wstring();
                    m_match = L"(\\s+)";
                }
            }
        }
    }

    void modify_filepath() const override {}
};

} // namespace Marvin

#endif // _H_FILE_REPLACE_STRATEGY_H_
