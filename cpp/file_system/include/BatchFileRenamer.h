#ifndef _H_BATCH_FILE_RENAMER_H_
#define _H_BATCH_FILE_RENAMER_H_

#include "FileRenameCommandBase.h"
#include "FileOverwritePrompt.h"

#include <vector>

namespace Marvin
{

/**
 * @brief Tool to rename a collection of paths according to the specified
 * options.
 */
class BatchFileRenamer
{
  //private:
  protected:
    // Paths to process.
    std::vector<std::string_view> m_paths;
    // Options to modify the paths.
    std::vector<std::string_view> m_options;
    // List of commands to process paths.
    std::vector<std::unique_ptr<FileRenameCommandBase>> m_commands_ptrs {};

    // Define which components are to be renamed (files, extensions,
    // directories.)
    enum class Targets : size_t
    {
        NONE = 0,
        FILES = 1,
        DIRECTORIES = 2,
        EXTENSIONS = 4,
    };

    // Define whether the paths are to be sorted and how.
    enum class Sorting : size_t
    {
        NONE = 0,
        BY_NAMES = 1,
        BY_NAMES_DESCENDING = 2,
        BY_SIZE = 4,
        BY_SIZE_DESCENDING = 8,
        BY_TIMESTAMP = 16,
        BY_TIMESTAMP_DESCENDING = 32,
    };

    // Flag to show help information.
    bool m_do_show_help {false};
    // Flag to process directories recursively.
    bool m_recursive {false};
    // Flag to output information.
    bool m_verbose {false};
    // Flag to do the actual renaming.
    bool m_do_modify {false};

    // Flag to do check overwriting of existing files.
    FileOverwritePrompt m_overwrite_prompt {};

    size_t m_targets {static_cast<size_t>(Targets::FILES)};
    size_t m_sorting {static_cast<size_t>(Sorting::NONE)};

    /**
     * @brief Show help information.
     */
    void m_show_help() const;

    /**
     * @brief Set current option.
     *
     * @param[in] i Index of the item in the list of options - might be
     * modified.
     */
    virtual void m_set_option(size_t& i);

    size_t m_build_target_flag(const bool are_directories) const;

    /**
     * @brief Sorting items by name.
     *
     * @param files File names to sort.
     * @param directories Folder names to sort.
     */
    void m_check_sorting_by_names(
        std::vector<std::pair<std::wstring, std::filesystem::path>>& files,
        std::vector<std::pair<std::wstring, std::filesystem::path>>&
            directories);

    /**
     * @brief Sorting items by size. Folders are not processed.
     *
     * @param files File names to sort.
     */
    void m_check_sorting_by_size(
        std::vector<std::pair<std::wstring, std::filesystem::path>>& files);

    /**
     * @brief Sorting items by timestamp.
     *
     * @param files File names to sort.
     * @param directories Folder names to sort.
     */
    void m_check_sorting_by_timestamp(
        std::vector<std::pair<std::wstring, std::filesystem::path>>& files,
        std::vector<std::pair<std::wstring, std::filesystem::path>>&
            directories);

    /**
     * @brief Process items in the directory or the file path.
     *
     * @param target_path Path to the directory or file.
     */
    void m_process_target(const std::string_view&);

    /**
     * @brief Process items (either files or directories).
     *
     * @param items Items to process.
     * @param are_directories Boolean flag whether the items are directories.
     */
    void m_process_items(
        std::vector<std::pair<std::wstring, std::filesystem::path>>& items,
        const bool = false);

  public:
    /**
     * @brief Construct a new Batch File Renamer:: Batch File Renamer object.
     *
     * @param paths Paths to process.
     * @param options Options to use for processing.
     */
    explicit BatchFileRenamer(const std::vector<std::string_view>&,
        const std::vector<std::string_view>&);

    virtual ~BatchFileRenamer() = default;

    /**
     * @brief Process for each directory
     */
    void run();
};

} // namespace Marvin

#endif // _H_BATCH_FILE_RENAMER_H_
