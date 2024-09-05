#ifndef _H_FILE_RENAME_STRATEGY_H_
#define _H_FILE_RENAME_STRATEGY_H_

#include <string>
#include <vector>

namespace Marvin
{

class FileRenameStrategy
{
  //private:
  protected:
    std::string_view m_path;
    std::vector<std::string_view> m_options;

  public:
    explicit FileRenameStrategy(const std::string_view& path,
        const std::vector<std::string_view>& options)
        : m_path {path}, m_options {options}
    {
    }

    virtual ~FileRenameStrategy() = default;

    virtual void modify_filepath() const = 0;
};

} // namespace Marvin

#endif // _H_FILE_RENAME_STRATEGY_H_
