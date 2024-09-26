#ifndef _H_MP3_BATCH_FILE_RENAMER_H_
#define _H_MP3_BATCH_FILE_RENAMER_H_

#include "MP3Tag.h"
#include "BatchFileRenamer.h"

#include <vector>

namespace Marvin
{

class MP3BatchFileRenamer
{
  private:
    std::vector<MP3Tag> m_items {};

    std::vector<std::string_view> m_paths;
    std::vector<std::string_view> m_options;

    // List of commands to process paths.
    std::vector<std::unique_ptr<FileRenameCommandBase>> m_commands_ptrs {};

    enum class Target : size_t
    {
        NONE = 0,
        TITLE = 1,
        ARTIST = 2,
        ALBUM = 4,
    };

    size_t m_target {std::to_underlying(Target::NONE)};

    // Flag to process directories recursively.
    bool m_recursive {false};
    // Flag to output information.
    bool m_verbose {false};
    // Flag to do the actual renaming.
    bool m_do_modify {false};

    // Flag to do check overwriting of existing files.
    FileOverwritePrompt m_overwrite_prompt {};

    void m_process_options();
    void m_process_items(const std::string_view& path);
    void m_add_if_valid(std::vector<MP3Tag>& items,
        const std::filesystem::directory_entry& item) const;

  protected:
    /**
     * @brief Set current option.
     *
     * @param[in] i Index of the item in the list of options - might be
     * modified.
     */
    //void m_set_option(size_t& i);

  public:
    explicit MP3BatchFileRenamer(const std::vector<std::string_view>&,
        const std::vector<std::string_view>&);

    void run();
};

} // namespace Marvin

#endif // _H_MP3_BATCH_FILE_RENAMER_H_
