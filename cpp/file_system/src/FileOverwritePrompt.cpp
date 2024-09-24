#include "FileOverwritePrompt.h"

#include <iostream>

namespace Marvin
{

FileOverwritePrompt::FileOverwritePrompt(const Mode& mode) : m_mode {mode} {}

void FileOverwritePrompt::set_mode(const Mode&& mode)
{
    m_mode = mode;
}

bool FileOverwritePrompt::is_overwriting(
    const std::filesystem::path& from, const std::filesystem::path& to)
{
    // If the file doesn't exist, then can safely "overwrite" (that is, copy or
    // move) the file to a new place. If the paths point to the same file, then
    // there is no sense of replacing file with itself.
    if (!std::filesystem::exists(to) || (from == to))
    {
        return true;
    }

    if (m_mode == FileOverwritePrompt::Mode::NO_TO_ALL)
    {
        return false;
    }

    if ((m_mode != FileOverwritePrompt::Mode::YES_TO_ALL)
        && (m_mode != FileOverwritePrompt::Mode::YES_TO_ALL_UPDATE))
    {
        // Use letters to process the answer:
        // 'n' - do not overwrite the current item;
        // 'o' - do not overwrite existing items;
        // 'y' - do overwrite the current item;
        // 'a' - do overwrite existing items;
        // 'u' - do overwrite the current item if newer;
        // 'w' - do overwrite existing items if newer;
        std::wcout
            << "Overwrite? (choose a letter) n = no, o = no to all, y = yes, a "
               "= yes to [a]ll, u = yes if newer, w = yes to all if newer\n";
        char answer {};
        std::cin >> answer;
        switch (answer)
        {

            case 'n':
                m_mode = FileOverwritePrompt::Mode::NO;
                break;

            case 'o':
                m_mode = FileOverwritePrompt::Mode::NO_TO_ALL;
                break;

            case 'y':
                m_mode = FileOverwritePrompt::Mode::YES;
                break;

            case 'a':
                m_mode = FileOverwritePrompt::Mode::YES_TO_ALL;
                break;

            case 'u':
                m_mode = FileOverwritePrompt::Mode::YES_UPDATE;
                break;

            case 'w':
                m_mode = FileOverwritePrompt::Mode::YES_TO_ALL_UPDATE;
                break;

            default:
                break;
        }
    }

    if ((m_mode == FileOverwritePrompt::Mode::YES)
        || (m_mode == FileOverwritePrompt::Mode::YES_TO_ALL))
    {
        return true;
    }

    if ((m_mode == FileOverwritePrompt::Mode::YES_UPDATE)
        || (m_mode == FileOverwritePrompt::Mode::YES_TO_ALL_UPDATE))
    {
        std::error_code err_code {};
        const auto stamp_from {
            std::filesystem::last_write_time(from, err_code)};
        if (err_code)
        {
            return false;
        }

        const auto stamp_to {std::filesystem::last_write_time(to, err_code)};
        if (err_code)
        {
            return false;
        }

        return stamp_from > stamp_to;
    }

    return false;
}

} // namespace Marvin
