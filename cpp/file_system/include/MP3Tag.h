#ifndef _H_MP3_TAG_H_
#define _H_MP3_TAG_H_

#include <string>
#include <map>
#include <vector>
#include <filesystem>

namespace Marvin
{

class MP3Tag
{
  private:
    std::string m_file_path;
    std::string m_id;
    std::vector<std::string_view> m_options;

    std::map<std::string, std::wstring> m_text_frames {};

    char m_major_version {};
    char m_revision_number {};
    char m_flags {};

    size_t m_extended_header_size {0};
    size_t m_tag_size {};

    // Flag to show help information.
    bool m_do_show_help {false};
    // Flag to process directories recursively.
    bool m_recursive {false};
    // Flag to output information.
    bool m_verbose {false};
    // Flag to do the actual renaming.
    bool m_do_modify {false};
    // Flag to determine whether the tag structure is valid.
    bool m_is_valid {false};

    bool m_read_header(std::ifstream&);

    bool m_read_frame(std::ifstream&);

    bool m_read_from_file(const std::string&);

    std::wstring m_encoding_as_text(const char) const;

  public:
    explicit MP3Tag(const std::string&, const std::vector<std::string_view>&);
    explicit MP3Tag(const std::filesystem::directory_entry&,
        const std::vector<std::string_view>&);

    std::filesystem::path get_path() const;
    std::wstring get_frame_text(const std::string& key) const;
    std::wstring get_title() const;
    std::wstring get_artist() const;
    std::wstring get_album() const;

    bool is_valid() const;

    void show_frames() const;
};

} // namespace Marvin

#endif // _H_MP3_TAG_H_
