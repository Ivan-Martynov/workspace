#ifndef _H_FILE_RENAME_STRATEGY_H_
#define _H_FILE_RENAME_STRATEGY_H_

#include <string>
#include <vector>

namespace Marvin
{

class FileRenameStrategy
{
  private:
    bool m_recursive {false};
    bool m_verbose {false};
    bool do_modify {false};

  protected:
    std::string_view m_path;
    std::vector<std::string_view> m_options;

  public:
    explicit FileRenameStrategy(const std::string_view& path,
        const std::vector<std::string_view>& options)
        : m_path {path}, m_options {options}
    {
        for (const auto& option : m_options)
        {
            if ((option == "-r") || (option == "-recursive"))
            {
                m_recursive = true;
            }
            else if ((option == "-v") || (option == "-verbose"))
            {
                m_verbose = true;
            }
            else if ((option == "-dm") || (option == "-do-modify"))
            {
                do_modify = true;
            }
        }
    }

    virtual ~FileRenameStrategy() = default;

    virtual void modify_filepath() const = 0;
};

} // namespace Marvin

#endif // _H_FILE_RENAME_STRATEGY_H_
