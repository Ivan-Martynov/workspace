#ifndef _H_BATCH_FILE_RENAMER_H_
#define _H_BATCH_FILE_RENAMER_H_

#include "FileRenameCommandInterface.hpp"

#include <vector>

namespace Marvin
{

class BatchFileRenamer
{
  private:
    std::vector<std::string_view> m_paths;
    std::vector<std::string_view> m_options;
    std::vector<std::unique_ptr<FileRenameCommandInterface>> m_commands_ptrs {};

    enum class Targets
    {
        NONE = 0,
        FILES = 1,
        DIRECTORIES = 2,
        EXTENSIONS = 4,
    };

    enum class Sorting
    {
        NONE = 0,
        BY_NAMES = 1,
        BY_NAMES_DESCENDING = 2,
        BY_SIZE = 4,
        BY_SIZE_DESCENDING = 8,
        BY_TIMESTAMP = 16,
        BY_TIMESTAMP_DESCENDING = 32,
    };

    bool m_recursive {false};
    bool m_verbose {false};
    bool do_modify {false};

    size_t m_targets {static_cast<size_t>(Targets::FILES)};
    size_t m_sorting {static_cast<size_t>(Sorting::NONE)};

    void process_directory(const std::string_view&);
    void process_items(std::vector<std::filesystem::path>&);

  public:
    explicit BatchFileRenamer(const std::vector<std::string_view>&,
        const std::vector<std::string_view>&);

    void run();
};

} // namespace Marvin

#endif // _H_BATCH_FILE_RENAMER_H_
