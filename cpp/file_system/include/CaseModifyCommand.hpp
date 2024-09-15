#ifndef _H_CASE_MODIFY_COMMAND_H_
#define _H_CASE_MODIFY_COMMAND_H_

#include "FileRenameCommandInterface.hpp"

namespace Marvin
{

/**
 * @brief Command to change the case of the path name: to uppercase, lowercase
 * or camel case.
 * 
 */
class CaseModifyCommand : public FileRenameCommandInterface
{
  private:
    char m_method;

  public:
    explicit CaseModifyCommand(const char);

    ~CaseModifyCommand() = default;

    explicit CaseModifyCommand(const CaseModifyCommand& other) = default;
    CaseModifyCommand& operator=(const CaseModifyCommand& other) = default;

    void modify(
        std::filesystem::path&, const size_t target_flag) const override;
};

} // namespace Marvin

#endif // _H_CASE_MODIFY_COMMAND_H_
