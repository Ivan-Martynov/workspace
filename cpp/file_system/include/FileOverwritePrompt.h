#ifndef H_INCLUDE_FILE_OVERWRITE_PROMPT_H
#define H_INCLUDE_FILE_OVERWRITE_PROMPT_H

/**
 * @file FileOverwritePrompt.h
 *
 * @brief Provides a prompt if the file to be copied or moved already exists.
 * In that case the user is asked whether to overwrite the existing file. The
 * possible answers are:
 * - No;
 * - No to all (do not ask again);
 * - Yes;
 * - Yes to all (do not ask again);
 * - Yes if the file is newer than the existing one;
 * - Yes to all if the file is newer than the existing one (do not ask again);
 *
 * @version 0.1
 */

#include <filesystem>

namespace Marvin
{
class FileOverwritePrompt
{
  public:
    /**
     * @brief Enumeration to list all possible options.
     */
    enum class Mode : size_t
    {
        NONE = 0, // Has not been defined yet.
        NO = 1,
        NO_TO_ALL = 2,
        YES = 4,
        YES_TO_ALL = 8,
        YES_UPDATE = 16,
        YES_TO_ALL_UPDATE = 32,
    };

  private:
    Mode m_mode;

  public:
    /**
     * @brief Construct a new File Overwrite Prompt object.
     *
     * @param[in] mode Mode specifying how to react on prompt.
     */
    explicit FileOverwritePrompt(const Mode& mode = Mode::NONE);

    /**
     * @brief Set the mode object.
     *
     * @param[in] mode Mode to set.
     */
    void set_mode(const Mode&& mode);

    /**
     * @brief Get the prompt mode object for two paths and check if the file is
     * to be overwritten.
     *
     * @param[in] from Item's path to copy/move.
     * @param[in] to Item's new path.
     * @return Mode Overwrite mode to determine whether the
     * @return true Overwrite the file.
     * @return false Do not overwrite the file.
     */
    bool is_overwriting(
        const std::filesystem::path& from, const std::filesystem::path& to);
};

} // namespace Marvin

#endif // H_INCLUDE_FILE_OVERWRITE_PROMPT_H
