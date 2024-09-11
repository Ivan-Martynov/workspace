#ifndef _H_FILE_REPLACE_STRATEGY_H_
#define _H_FILE_REPLACE_STRATEGY_H_

#include "FileRenameStrategy.hpp"

#include <filesystem>
#include <iostream>
#include <regex>

namespace Marvin
{

class FileReplaceStrategy : public FileRenameStrategy
{
  private:
    std::wstring m_match {L""};
    std::wstring m_replacement {L""};

  public:
    FileReplaceStrategy(const std::vector<std::string>& options)
        : FileRenameStrategy(options)
    {
        for (size_t i {0}; i < m_options.size(); ++i)
        {
            const auto option {m_options[i]};
            if ((option == "-rur") || (option == "-replace-using-regex")
                || (option == "-rsw") || (option == "-replace-substring-with"))
            {
                if (++i < m_options.size())
                {
                    // A trick to convert string to wstring.
                    m_match = std::filesystem::path(m_options[i]).wstring();
                }

                if (++i < m_options.size())
                {
                    m_replacement
                        = std::filesystem::path(m_options[i]).wstring();
                }
            }
            else if ((option == "-rww")
                     || (option == "-replace-whitespace-with"))
            {
                m_match = L"(\\s+)";
                if (++i < m_options.size())
                {
                    m_replacement
                        = std::filesystem::path(m_options[i]).wstring();
                }
            }
            else if ((option == "-rwwu")
                     || (option == "-replace-whitespace-with-underscore"))
            {
                m_match = L"(\\s+)";
                m_replacement = L"_";
            }
        }
    }

    void modify_entry(std::filesystem::directory_entry& entry) override
    {
        if (m_match.empty())
        {
            return;
        }

        std::filesystem::path path {entry.path()};

        const auto file_name {std::regex_replace(
            path.filename().wstring(), std::wregex(m_match), m_replacement)};
        const auto modified_path {path.parent_path() / file_name};

        const auto extension {path.extension()};

        const bool modified {path != modified_path};

        std::wcout << entry << L" is directory? = " << entry.is_directory() << L"\n";

        if (m_verbose && modified)
        {
            path.replace_filename(file_name);
            std::wcout << path << L" => " << modified_path;
            std::wcout << L"\n";
        }

        if (modified && do_modify)
        {
            try
            {
                std::filesystem::rename(path, modified_path);
            }
            catch (const std::filesystem::filesystem_error& err)
            {
                std::cerr << "what(): " << err.what() << "\n";
            }
        }
    }
};

} // namespace Marvin

#endif // _H_FILE_REPLACE_STRATEGY_H_
