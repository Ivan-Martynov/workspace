#ifndef _H_I_FILE_DECORATOR_H_
#define _H_I_FILE_DECORATOR_H_

namespace Marvin
{

class IFileDecorator
{
  public:
    virtual ~IFileDecorator() = default;

    virtual void execute() const = 0;
};

} // namespace Marvin

#endif // _H_I_FILE_DECORATOR_H_
