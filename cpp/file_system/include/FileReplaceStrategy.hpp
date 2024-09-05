#ifndef _H_FILE_REPLACE_STRATEGY_H_
#define _H_FILE_REPLACE_STRATEGY_H_

#include "FileRenameStrategy.hpp"

namespace Marvin
{

class FileReplaceStrategy: public FileRenameStrategy
{
  public:
    FileReplaceStrategy(const std::string_view& path,
        const std::vector<std::string_view>& options)
        : FileRenameStrategy(path, options)
    {
    }

    void modify_filepath() const override {}
};

} // namespace Marvin

#endif // _H_FILE_REPLACE_STRATEGY_H_
