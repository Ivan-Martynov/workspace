#include "CaseModifyCommand.h"

#include <algorithm>

namespace Marvin
{

/**
 * @brief Construct a new Case Modify Command:: Case Modify Command object.
 *
 * @param c Character to define the modification method: 'u' - uppercase, 'l' -
 * lowercase, 'c' - camel case, 'p' - Pascal case.
 */
CaseModifyCommand::CaseModifyCommand(const char c) : m_method {c} {}

/**
 * @brief Modify path using target flag.
 *
 * @param path Path to an item.
 * @param target_flag Flag to determine which part of the item to modify.
 */
void CaseModifyCommand::modify(
    std::filesystem::path& path, const size_t target_flag) const
{
    std::wstring file_name {get_filepart(path, target_flag)};

    if (m_method == 'u') // uppecase
    {
        std::transform(
            file_name.begin(), file_name.end(), file_name.begin(), ::towupper);
    }
    else if (m_method == 'l') // lowercase
    {
        std::transform(
            file_name.begin(), file_name.end(), file_name.begin(), ::towlower);
    }
    else if ((m_method == 'c') || (m_method == 'p')) // camel or Pascal case
    {
        bool expect_first_letter {m_method == 'p'};
        //bool expect_first_letter {true};
        std::transform(file_name.begin(), file_name.end(), file_name.begin(),
            [&expect_first_letter](wchar_t c)
            {
                if (std::iswalpha(c))
                {
                    if (expect_first_letter)
                    {
                        expect_first_letter = false;
                        return std::towupper(c);
                    }
                    else
                    {
                        return std::towlower(c);
                    }
                }
                else
                {
                    expect_first_letter = true;
                    return static_cast<wint_t>(c);
                }
            });
    }

    modify_filename(path, file_name, target_flag);
}

} // namespace Marvin
