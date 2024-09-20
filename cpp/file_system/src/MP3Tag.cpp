#include "MP3Tag.h"
#include "StringHelper.h"
#include "SyncsafeIntConverter.h"

#include <fstream>
#include <iomanip>
#include <iostream>

namespace Marvin
{

MP3Tag::MP3Tag(const std::filesystem::directory_entry& entry,
    const std::vector<std::string_view>& options)
    : m_file_path {entry.path().string()}, m_id(3, '\0'), m_options {options}
{
    for (const auto& option : m_options)
    {
        if ((option == "-h") || (option == "-help"))
        {
            m_do_show_help = true;
        }
        else if ((option == "-r") || (option == "-recursive"))
        {
            m_recursive = true;
        }
        else if ((option == "-v") || (option == "-verbose"))
        {
            m_verbose = true;
        }
    }

    if (!entry.is_regular_file())
    {
        m_is_valid = false;
        return;
    }

    if (!StringHelper::equal_ignore_case(
            entry.path().extension().string(), ".mp3"))
    {
        if (m_verbose)
        {
            std::wcerr << entry.path() << L" is not an mp3 file.\n";
        }
        m_is_valid = false;
        return;
    }

    if (m_read_from_file(entry.path().string()))
    {
        m_is_valid = true;
    }
}

MP3Tag::MP3Tag(
    const std::string& file_path, const std::vector<std::string_view>& options)
    : m_file_path {file_path}, m_id(3, '\0'), m_options {options}
{
    if (m_read_from_file(file_path))
    {
        m_is_valid = true;
    }

    for (const auto& option: m_options)
    {
        if ((option == "-h") || (option == "-help"))
        {
            m_do_show_help = true;
        }
        else if ((option == "-r") || (option == "-recursive"))
        {
            m_recursive = true;
        }
        else if ((option == "-v") || (option == "-verbose"))
        {
            m_verbose = true;
        }
    }
}

bool MP3Tag::is_valid() const
{
    return m_is_valid;
}

bool MP3Tag::m_read_header(std::ifstream& file_stream)
{
    if (!file_stream.read(m_id.data(), 3))
    {
        if (m_verbose)
        {
            std::wcerr << L"Failed to read mp3 tag identifier.\n";
        }
        return false;
    }

    if (!file_stream.get(m_major_version))
    {
        if (m_verbose)
        {
            std::wcerr << L"Failed to read mp3 header major version.\n";
        }
        return false;
    }

    if (!file_stream.get(m_revision_number))
    {
        if (m_verbose)
        {
            std::wcerr << L"Failed to read mp3 header revision number.\n";
        }
        return false;
    }

    if ((m_major_version != 4) && (m_major_version != 3))
    {
        if (m_verbose)
        {
            std::wcerr << L"Not a 2.3 or 2.4 header: "
                       << static_cast<int>(m_major_version) << L"\n";
        }
        return false;
    }

    if (!file_stream.get(m_flags))
    {
        if (m_verbose)
        {
            std::wcerr << L"Failed to read mp3 header flags.\n";
        }
        return false;
    }

    std::string tag_size_str(4, '\0');
    file_stream.read(tag_size_str.data(), 4);

    m_tag_size = SyncsafeIntConverter::syncsafe_decode_from_bytes(
        tag_size_str.c_str(), 4);

    if ((m_flags >> 6) & 1)
    {
        std::string extended_tag_str(4, '\0');
        file_stream.read(extended_tag_str.data(), 4);
        if (extended_tag_str.length() < 4)
        {
            return false;
        }

        m_extended_header_size
            = SyncsafeIntConverter::syncsafe_decode_from_bytes(
                extended_tag_str.c_str(), 4);
    }

    return true;
}

std::wstring MP3Tag::m_encoding_as_text(const char encoding_char) const
{
    switch (encoding_char)
    {

        case 0:
            return L"ISO-8859-1 [ISO-8859-1]";
        case 1:
            return L"UTF-16 [UTF-16] encoded Unicode [UNICODE] with BOM";
        case 2:
            return L"UTF-16BE [UTF-16] encoded Unicode [UNICODE] without BOM";
        case 3:
            return L"UTF-8 [UTF-8] encoded Unicode [UNICODE]";
        default:
            return L"";
    }
}

bool MP3Tag::m_read_frame(std::ifstream& file_stream)
{
    constexpr size_t frame_id_size {4};
    std::string frame_id_buffer(frame_id_size, '\0');
    file_stream.read(frame_id_buffer.data(), frame_id_size);

    if (frame_id_buffer.length() < frame_id_size)
    {
        return false;
    }

    std::string frame_size_buffer(4, '\0');
    file_stream.read(frame_size_buffer.data(), 4);
    const size_t frame_size {SyncsafeIntConverter::syncsafe_decode_from_bytes(
        frame_size_buffer.c_str(), 4)};

    std::string frame_flags_buffer(2, '\0');
    file_stream.read(frame_flags_buffer.data(), 2);

    if (frame_id_buffer.front() == 'T') // Text frame.
    {
        const char encoding_char {static_cast<char>(file_stream.get())};
        const size_t value {SyncsafeIntConverter::syncsafe_decode_from_bytes(
            &encoding_char, 1)};
        std::string frame_str(frame_size - 1, '\0');
        file_stream.read(frame_str.data(), frame_size - 1);

        if (value == 0)
        {
            std::wcout << L"[0]: Test ISO-8859-1 [ISO-8859-1]: ";

            std::string s {StringHelper::iso_8859_1_to_string(frame_str)};
            for (const auto c : s)
            {
                std::wcout << c;
            }
            if (s.empty())
            {
                std::wcout << L"Empty string\n";
            }
            std::wcout << L" and maybe " << StringHelper::string_to_wstring(s)
                       << L"\n";
        }
        else if (value == 1)
        {
            // std::u16string s16 {
            //     std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,
            //         char16_t> {}
            //         .from_bytes(frame_str.data())};
            // std::wcout << L"UTF-16 conversion produced " << s16.size()
            //            << L" code units\n";
            std::wcout << L"[1]: Test u16: ";
            for (const auto c : frame_str)
            {
                std::wcout << c;
            }
            std::wcout << L"\n";
        }

        if (m_verbose)
        {
            //std::wcout << L"Text frame: encoding = "
            //           << m_encoding_as_text(encoding_char) << L": id = "
            //           << StringHelper::string_to_wstring(frame_id_buffer)
            //           << "; text = "
            //           << StringHelper::string_to_wstring(frame_str) << L"\n";
        }

        m_text_frames[frame_id_buffer]
            = StringHelper::string_to_wstring(frame_str);
    }
    else
    {
        file_stream.seekg(frame_size, std::ios_base::cur);
    }

    return true;
}

/**
 * @brief Read mp3 tag data from a file.
 *
 * @param[in] file_path Path to the file.
 * @return true MP3 data successfully extracted and false otherwise.
 */
bool MP3Tag::m_read_from_file(const std::string& file_path)
{
    if (m_verbose)
    {
        std::wcout << L"Trying to get mp3 data for "
                   << StringHelper::string_to_wstring(file_path) << L"\n";
    }

    std::ifstream file_stream {file_path, std::ios::binary | std::ios::in};
    if (!file_stream)
    {
        std::wcerr << L"Failed reading file "
                   << StringHelper::string_to_wstring(file_path) << L"\n";
        return false;
    }

    if (!m_read_header(file_stream))
    {
        return false;
    }

    while (file_stream.tellg() < static_cast<long int>(m_tag_size))
    {
        if (!m_read_frame(file_stream))
        {
            return false;
        }
    }

    return true;
}

std::filesystem::path MP3Tag::get_path() const
{
    return m_file_path;
}

std::wstring MP3Tag::get_frame_text(const std::string& key) const
{
    return m_text_frames.contains(key) ? m_text_frames.at(key) : L"";
}

std::wstring MP3Tag::get_title() const
{
    return get_frame_text("TIT2");
}

std::wstring MP3Tag::get_artist() const
{
    return get_frame_text("TPE1");
}

std::wstring MP3Tag::get_album() const
{
    return get_frame_text("TALB");
}

void MP3Tag::show_frames() const
{
    if (!m_verbose)
    {
        return;
    }
    for (const auto& p : m_text_frames)
    {
        if (!p.second.empty())
        {
            std::wcout << L"[" << StringHelper::string_to_wstring(p.first)
                       << L"] = " << p.second << L"\n";
        }
    }
}

} // namespace Marvin
