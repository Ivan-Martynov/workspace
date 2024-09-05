#ifndef _H_FILENAME_REPLACE_DECORATOR_H_
#define _H_FILENAME_REPLACE_DECORATOR_H_

#include "FileDecorator.hpp"

#include <iostream>

namespace Marvin
{

class FilenameReplaceDecorator: public FileDecorator
{
  public:
    explicit FilenameReplaceDecorator(IFileDecorator* decorator_ptr)
        : FileDecorator {decorator_ptr}
    {
    }

    ~FilenameReplaceDecorator()
    {
        std::wcout << L"Destroying FilenameReplaceDecorator\n";
    }

    explicit FilenameReplaceDecorator(const FilenameReplaceDecorator& other)
        = default;
    FilenameReplaceDecorator& operator=(const FilenameReplaceDecorator& other)
        = default;

    void execute() const override
    {
        std::wcout << L"Calling FilenameReplaceDecorator\n";
        FileDecorator::execute();
    }
};


} // namespace Marvin

#endif // _H_FILENAME_REPLACE_DECORATOR_H_
