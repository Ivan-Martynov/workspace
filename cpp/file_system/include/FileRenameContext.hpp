#ifndef _H_FILE_RENAME_CONTEXT_H_
#define _H_FILE_RENAME_CONTEXT_H_

#include "FileRenameStrategy.hpp"

#include <memory>

namespace Marvin
{

class FileRenameContext
{
  private:
    std::string m_path;
    std::unique_ptr<FileRenameStrategy> m_strategy_ptr;

  public:
    explicit FileRenameContext(const std::string& dir_path,
        std::unique_ptr<FileRenameStrategy>&& strategy_ptr = {})
        : m_path {dir_path}, m_strategy_ptr {std::move(strategy_ptr)}
    {
    }

    void set_strategy(std::unique_ptr<FileRenameStrategy>&& strategy_ptr)
    {
        m_strategy_ptr = std::move(strategy_ptr);
    }

    void modify() const
    {
        if (m_strategy_ptr)
        {
            m_strategy_ptr->modify_files_in_dir(m_path);
        }
    }
};

} // namespace Marvin

#endif // _H_FILE_RENAME_CONTEXT_H_
