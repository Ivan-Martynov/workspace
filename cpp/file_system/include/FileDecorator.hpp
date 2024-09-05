#ifndef _H_FILE_DECORATOR_H_
#define _H_FILE_DECORATOR_H_

#include "IFileDecorator.hpp"

namespace Marvin
{

class FileDecorator: public IFileDecorator
{
  protected:
    IFileDecorator* m_decorator_ptr;

  public:
    FileDecorator(IFileDecorator* decorator_ptr)
        : m_decorator_ptr {decorator_ptr}
    {
    }

    explicit FileDecorator(const FileDecorator&) = default;
    FileDecorator& operator=(const FileDecorator&) = default;

    void execute() const override
    {
        m_decorator_ptr->execute();
    }
};

} // namespace Marvin

#endif // _H_FILE_DECORATOR_H_
