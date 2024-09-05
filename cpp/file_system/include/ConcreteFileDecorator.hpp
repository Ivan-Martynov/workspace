#ifndef _H_CONCRETE_FILE_DECORATOR_H_
#define _H_CONCRETE_FILE_DECORATOR_H_

#include "IFileDecorator.hpp"

#include <iostream>
#include <string>

namespace Marvin
{

class ConcreteFileDecorator: public IFileDecorator
{
  private:
    std::string m_path;

  public:
    explicit ConcreteFileDecorator(const std::string& path) : m_path {path} {}

    virtual ~ConcreteFileDecorator()
    {
        std::wcout << L"Destroying ConcreteFileDecorator\n";
    };

    virtual void execute() const override
    {
        std::wcout << L"Calling ConcreteFileDecorator\n";
    }
};

} // namespace Marvin

#endif // _H_CONCRETE_FILE_DECORATOR_H_
