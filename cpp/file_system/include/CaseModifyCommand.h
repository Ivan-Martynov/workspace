#ifndef _H_CASE_MODIFY_COMMAND_H_
#define _H_CASE_MODIFY_COMMAND_H_

#include "FileRenameCommandBase.h"

namespace Marvin
{

/**
 * @brief Command to change the case of the path name: to uppercase, lowercase
 * or camel case.
 *
 */
class CaseModifyCommand : public FileRenameCommandBase
{
  private:
    char m_method;

  public:
    /**
     * @brief Construct a new Case Modify Command:: Case Modify Command object.
     *
     * @param c Character to define the modification method: 'u' - uppercase,
     * 'l' - lowercase, 'c' - camel case.
     */
    explicit CaseModifyCommand(const char);

    ~CaseModifyCommand() = default;

    explicit CaseModifyCommand(const CaseModifyCommand& other) = default;
    CaseModifyCommand& operator=(const CaseModifyCommand& other) = default;

    /**
     * @brief Modify path using target flag.
     *
     * @param path Path to an item.
     * @param target_flag Flag to determine which part of the item to modify.
     */
    void modify(
        std::filesystem::path&, const size_t target_flag) const override;
};

} // namespace Marvin

#endif // _H_CASE_MODIFY_COMMAND_H_
