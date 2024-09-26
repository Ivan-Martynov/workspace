#ifndef _H_FILE_ORGANIZER_H_
#define _H_FILE_ORGANIZER_H_

#include <vector>
#include <string>
#include <filesystem>

#include "FileOverwritePrompt.h"

namespace Marvin
{

class FileOrganizer
{
  private:
    // Paths to process.
    std::vector<std::string_view> m_paths;
    // Options to modify the paths.
    std::vector<std::string_view> m_options;

    // Flag to show help information.
    bool m_do_show_help {false};
    // Flag to output information.
    bool m_verbose {false};
    // Flag whether to copy files.
    bool m_do_copy {false};
    // Flag whether to move files.
    bool m_do_move {false};

    FileOverwritePrompt m_overwrite_prompt {};

    enum class FolderTarget : size_t
    {
        NONE = 0,
        YEAR = 1,
        MONTH = 2,
        DAY = 4,
        HOUR = 8,
        MINUTE = 16,
        SECOND = 32,
    };

    size_t m_folder_target {static_cast<size_t>(FolderTarget::NONE)};

    std::filesystem::path build_target_path(
        const std::filesystem::directory_entry&);
    void actual_copy_or_move(
        const std::filesystem::path&, const std::filesystem::path&);

    void process_path(const std::string_view&);
    void process_item(const std::filesystem::directory_entry&);

  public:
    explicit FileOrganizer(const std::vector<std::string_view>&,
        const std::vector<std::string_view>&);

    void run();
};

} // namespace Marvin

#endif // _H_FILE_ORGANIZER_H_
