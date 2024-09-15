#ifndef _H_STRING_APPEND_COMMAND_H_
#define _H_STRING_APPEND_COMMAND_H_

#include "FileRenameCommandInterface.hpp"

namespace Marvin
{

class StringAppendCommand : public FileRenameCommandInterface
{
  private:
    enum class Method
    {
        NONE,
        APPEND,
        PREPEND,
    };

    enum class Appendage
    {
        NONE,
        NUMBER,
        TIMESTAMP,
        CURRENT_TIME,
    };

    Method m_method {Method::NONE};
    Appendage m_appendage {Appendage::NONE};

    void append_numbers(std::vector<std::filesystem::path>&) const;
    void append_timestamp(std::vector<std::filesystem::path>& ) const;

  public:
    explicit StringAppendCommand(const char, const char);

    ~StringAppendCommand() = default;

    explicit StringAppendCommand(const StringAppendCommand& other) = default;
    StringAppendCommand& operator=(const StringAppendCommand& other) = default;

    void modify(std::filesystem::path&, const size_t) const override {}
    void modify(
        std::vector<std::filesystem::path>&, const size_t) const override;
};

} // namespace Marvin

#endif // _H_STRING_APPEND_COMMAND_H_
