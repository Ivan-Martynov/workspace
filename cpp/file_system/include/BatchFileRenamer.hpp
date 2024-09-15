#ifndef _H_BATCH_FILE_RENAMER_H_
#define _H_BATCH_FILE_RENAMER_H_

#include "FileRenameCommandInterface.hpp"

#include <vector>

namespace Marvin
{

/**
 * @brief Tool to rename a collection of paths according to the specified
 * options.
 */
class BatchFileRenamer
{
  private:
    // Paths to process.
    std::vector<std::string_view> m_paths;
    // Options to modify the paths.
    std::vector<std::string_view> m_options;
    // List of commands to process paths.
    std::vector<std::unique_ptr<FileRenameCommandInterface>> m_commands_ptrs {};

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

    enum class Overwrite
    {
        NONE,
        NO,
        YES,
    };

    // Flag to process directories recursively.
    bool m_recursive {false};
    // Flag to output information.
    bool m_verbose {false};
    // Flag to do the actual renaming.
    bool do_modify {false};
    // Flag to do check overwriting of existing files.
    Overwrite m_do_overwrite {Overwrite::NONE};

    size_t m_targets {static_cast<size_t>(Targets::FILES)};
    size_t m_sorting {static_cast<size_t>(Sorting::NONE)};

    void check_sorting_by_names(std::vector<std::filesystem::path>&,
        std::vector<std::filesystem::path>&);
    void check_sorting_by_size(std::vector<std::filesystem::path>&);
    void check_sorting_by_timestamp(std::vector<std::filesystem::path>&,
        std::vector<std::filesystem::path>&);

    void process_directory(const std::string_view&);
    void process_items(std::vector<std::filesystem::path>&, const bool = false);

  public:
    explicit BatchFileRenamer(const std::vector<std::string_view>&,
        const std::vector<std::string_view>&);

    void run();
};

} // namespace Marvin

#endif // _H_BATCH_FILE_RENAMER_H_
