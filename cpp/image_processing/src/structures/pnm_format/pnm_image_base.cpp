#include "structures/pnm_format/pnm_image_base.h"

#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>

namespace Marvin
{

namespace PNM_Format
{

// Using unnamed namespace to make inner content local to this file.
namespace
{

/**
 * @brief General settings for pnm images.
 */
namespace Settings
{

static constexpr int max_line_length {70};
static constexpr int absolute_max_value {65535}; // 2^16 - 1

} // namespace Settings

template <pnm_suitable T>
class pnm_image_traits;

/**
 * @brief Structure to contain information about a pnm image: typically, the id
 * from the header, width, height and max value.
 *
 * @remarks Black-white images do not contain max value in a header, but it is
 * still present in this structure. Typically set as one.
 */
struct PNMImageInfo
{
    using size_type = int;

    /**
     * @brief Type of image, representing format:
     * - P1 - plain black and white.
     * - P2 - plain grayscale.
     * - P3 - plain rgb.
     * - P4 - raw (binary) black and white.
     * - P5 - raw (binary) grayscale.
     * - P6 - raw (binary) rgb.
     *
     * max_type used as the count of types. Can also be used to mark the default
     * and/or error value.
     *
     * @todo Add P7.
     *
     */
    enum class Type
    {
        P1,
        P2,
        P3,
        P4,
        P5,
        P6,

        max_type,
    };

    /**
     * @brief Array containing names representing header ids.
     */
    static constexpr std::array<std::string_view,
        std::to_underlying(Type::max_type)>
        type_names {"P1", "P2", "P3", "P4", "P5", "P6"};

    PNMImageInfo() = default;

    template <class T>
    PNMImageInfo(Type image_type, const PNMImageTemplated<T>& image)
        : type {image_type}, width {image.width()}, height {image.height()},
          max_value {image.max_value()}
    {
    }

    template <class T>
    PNMImageInfo(const PNMImageTemplated<T>& image, bool is_raw)
        : PNMImageInfo {is_raw ? pnm_image_traits<T>::raw_type()
                               : pnm_image_traits<T>::plain_type(),
              image}
    //                       , width {image.width()}, height {image.height()},
    //         max_value {image.max_value()}
    {
    }

    /**
     * @brief Convert string to header type.
     *
     * @param[in] format_id String representing header id.
     * @return constexpr std::optional<Type> Type representing image format or
     * nullopt for illegal format id.
     */
    static constexpr std::optional<Type> type_from_string(
        std::string_view format_id)
    {
        for (size_t i {0}; i < type_names.size(); ++i)
        {
            if (type_names[i] == format_id)
            {
                return static_cast<Type>(i);
            }
        }

        return {};
    }

    static constexpr bool is_blackwhite(Type type)
    {
        return (type == Type::P1) || (type == Type::P4);
    }

    static constexpr bool is_grayscale(Type type)
    {
        return (type == Type::P2) || (type == Type::P5);
    }

    static constexpr bool is_rgb(Type type)
    {
        return (type == Type::P3) || (type == Type::P6);
    }

    static constexpr bool is_plain(Type type)
    {
        return (type == Type::P1) || (type == Type::P2) || (type == Type::P3);
    }

    static constexpr bool is_raw(Type type)
    {
        return (type == Type::P4) || (type == Type::P5) || (type == Type::P6);
    }

    Type type {};
    size_type width {};
    size_type height {};
    size_type max_value {};
};

template <>
class pnm_image_traits<BlackWhiteColor>
{
  public:
    static constexpr auto default_max_value() { return 1; }
    static constexpr auto channel_count() { return 1; }
    static constexpr auto plain_type() { return PNMImageInfo::Type::P1; }
    static constexpr auto raw_type() { return PNMImageInfo::Type::P4; }
};

template <>
class pnm_image_traits<GrayScaleColor>
{
  public:
    static constexpr auto default_max_value() { return 255; }
    static constexpr auto channel_count() { return 1; }
    static constexpr auto plain_type() { return PNMImageInfo::Type::P2; }
    static constexpr auto raw_type() { return PNMImageInfo::Type::P5; }
};

template <>
class pnm_image_traits<RGBColor>
{
  public:
    static constexpr auto default_max_value() { return 255; }
    static constexpr auto channel_count() { return 3; }
    static constexpr auto plain_type() { return PNMImageInfo::Type::P3; }
    static constexpr auto raw_type() { return PNMImageInfo::Type::P6; }
};

/**
 * @brief Structure for a pnm image header, containing the image info and header
 * comments.
 */
struct PNMImageHeader
{
    using size_type = PNMImageInfo::size_type;

    PNMImageInfo info {};
    HeaderComments comments {};

    PNMImageHeader() = default;
    PNMImageHeader(std::istream& stream);
};

/**
 * @brief The idea is to place most of the functions related to read/write
 * operations for images into a separate place.
 */
namespace FileOperations
{

std::string skip_whitespace_and_comment(std::istream& stream)
{
    std::string comment {};
    while (true)
    {
        if (const auto c {stream.get()}; c == '#')
        {
            while (std::isspace(stream.peek()))
            {
                stream.get();
            }
            std::string line {};
            std::getline(stream, line);
            if (!line.empty())
            {
                comment += line + "\n";
            }
            // stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (!std::isspace(c))
        {
            // Place the last valid character back to the stream.
            stream.unget();
            break;
        }
    }

    return comment;
}

bool reopen_file(std::ifstream& file_stream, const char* const file_path,
    std::ios_base::openmode open_mode = std::ios_base::in)
{
    file_stream.close();
    file_stream.open(file_path, open_mode);

    if (!file_stream.good())
    {
        std::cerr << "Failed to reopen file " << file_path << " for reading.\n";
        return false;
    }
    else
    {
        return true;
    }
}

template <pnm_suitable T>
void read_raster_plain(std::istream& stream, PNMImageTemplated<T>& image)
{
    for (auto& color : image.values())
    {
        if (!(stream >> color))
        {
            std::cerr << "Failed to read plain color from stream.\n";
            return;
        }
    }
}

template <pnm_suitable T>
void read_raster_raw(std::istream&, PNMImageTemplated<T>&);

template <>
void read_raster_raw<BlackWhiteColor>(
    std::istream& stream, PNMImageTemplated<BlackWhiteColor>& image)
{
    using size_type = PNMImageTemplated<BlackWhiteColor>::size_type;
    size_type pixel_index {0};
    const auto add_pixel_lambda {[&stream, &pixel_index, &image](int bit_count)
        {
            const auto byte_value {stream.get()};
            auto shift {8};
            while (bit_count--)
            {
                // Extract most significant bits first.
                image[pixel_index++] = byte_value & 1 << --shift;
            }
        }};

    const auto row_value_count {image.width() / 8};
    const int last_value_bit_count {static_cast<int>(image.width() % 8)};

    for (size_type row {0}; row < image.height(); ++row)
    {
        for (size_type i {0}; i < row_value_count; ++i)
        {
            add_pixel_lambda(8);
        }

        if (last_value_bit_count)
        {
            add_pixel_lambda(last_value_bit_count);
        }
    }
}

int ppm_raw_gray_value(std::istream& stream)
{
    const auto first {stream.get()};
    const auto second {stream.get()};
    return (first << 8) | second;
}

// TODO: check that reading from stream succeeds.
template <>
void read_raster_raw<GrayScaleColor>(
    std::istream& stream, PNMImageTemplated<GrayScaleColor>& image)
{
    if (image.max_value() > 0xFF)
    {
        std::ranges::for_each(image.values(),
            [&stream](auto& p) { p = ppm_raw_gray_value(stream); });
    }
    else
    {
        std::ranges::for_each(
            image.values(), [&stream](auto& p) { p = stream.get(); });
    }
}

template <>
void read_raster_raw<RGBColor>(
    std::istream& stream, PNMImageTemplated<RGBColor>& image)
{
    if (image.max_value() > 0xFF)
    {
        std::ranges::for_each(image.values(),
            [&stream](auto& p)
            {
                const auto red {ppm_raw_gray_value(stream)};
                const auto green {ppm_raw_gray_value(stream)};
                const auto blue {ppm_raw_gray_value(stream)};
                p = {red, green, blue};
            });
    }
    else
    {
        std::ranges::for_each(image.values(),
            [&stream](auto& p)
            {
                const auto red {stream.get()};
                const auto green {stream.get()};
                const auto blue {stream.get()};
                p = {red, green, blue};
            });
    }
}

template <pnm_suitable T>
void color_to_ostream_plain(
    std::ostream& stream, const T& color, int item_width);

template <>
void color_to_ostream_plain<BlackWhiteColor>(
    std::ostream& stream, const BlackWhiteColor& color, int item_width)
{
    stream << std::setw(item_width) << color.value();
}

template <>
void color_to_ostream_plain<GrayScaleColor>(
    std::ostream& stream, const GrayScaleColor& color, int item_width)
{
    stream << std::setw(item_width) << color.value();
}

template <>
void color_to_ostream_plain<RGBColor>(
    std::ostream& stream, const RGBColor& color, int item_width)
{
    stream << std::setw(item_width) << color.red() << ' '
           << std::setw(item_width) << color.green() << ' '
           << std::setw(item_width) << color.blue();
}

template <pnm_suitable T>
void write_raster_plain(std::ostream& stream, const PNMImageTemplated<T>& image)
{
    using size_type = PNMImageTemplated<T>::size_type;

    stream << std::left;
    const auto item_width {
        static_cast<size_type>(std::to_string(image.max_value()).length())};
    const auto multiplier {pnm_image_traits<T>::channel_count()};
    const auto width_step {item_width * multiplier};
    const auto stride {width_step + multiplier};
    const auto line_limit {
        std::min(Settings::max_line_length, image.width() * stride - 1)};

    size_type i {0};
    FileOperations::color_to_ostream_plain(stream, image[i], item_width);

    size_type current_width_pos {width_step};
    const auto n {image.width() * image.height()};
    while (++i < n)
    {
        current_width_pos += stride;
        const bool next_line {current_width_pos > line_limit};

        if (next_line)
        {
            current_width_pos = width_step;
            stream << '\n';
        }
        else
        {
            stream << ' ';
        }

        FileOperations::color_to_ostream_plain(stream, image[i], item_width);
    }
}

template <pnm_suitable T>
void write_raster_raw(std::ostream&, const PNMImageTemplated<T>&);

template <>
void write_raster_raw<BlackWhiteColor>(
    std::ostream& stream, const PNMImageTemplated<BlackWhiteColor>& image)
{
    using size_type = PNMImageTemplated<BlackWhiteColor>::size_type;
    size_type pixel_index {0};
    auto add_pixel_lambda {
        [&stream, &image, &pixel_index](size_type col, int bit_count)
        {
            unsigned char byte_value {};
            unsigned char shift {8};
            while (bit_count--)
            {
                // The most signficant bit is place first.
                byte_value |= image[pixel_index++].value() << --shift;
            }

            stream << byte_value;

            return col;
        }};

    const auto row_value_count {image.width() / 8};
    const int last_value_bit_count {static_cast<int>(image.width() % 8)};
    for (size_type row {0}; row < image.height(); ++row)
    {
        size_type col {0};
        for (size_type i {0}; i < row_value_count; ++i)
        {
            col = add_pixel_lambda(col, 8);
        }

        if (last_value_bit_count)
        {
            col = add_pixel_lambda(col, last_value_bit_count);
        }
    }
}

template <>
void write_raster_raw<GrayScaleColor>(
    std::ostream& stream, const PNMImageTemplated<GrayScaleColor>& image)
{
    if (image.max_value() > 0xFF)
    {
        for (const auto& color : image.values())
        {
            stream << static_cast<unsigned char>((color.value() >> 8) & 0xFF);
            stream << static_cast<unsigned char>(color.value() & 0xFF);
        }
    }
    else
    {
        for (const auto& color : image.values())
        {
            stream << static_cast<unsigned char>(color.value());
        }
    }
}

template <>
void write_raster_raw<RGBColor>(
    std::ostream& stream, const PNMImageTemplated<RGBColor>& image)
{
    if (image.max_value() > 0xFF)
    {
        for (const auto& color : image.values())
        {
            for (auto v : {color.red(), color.green(), color.blue()})
            {
                stream << static_cast<char>((v >> 8) & 0xFF);
                stream << static_cast<char>(v & 0xFF);
            }
        }
    }
    else
    {
        for (const auto& color : image.values())
        {
            for (auto v : {color.red(), color.green(), color.blue()})
            {
                stream << static_cast<char>(v);
            }
        }
    }
}

static void m_add_comment(std::ostream& stream, std::string_view comment)
{
    size_t word_start {comment.find_first_not_of(' ')};
    if (word_start == std::string_view::npos)
    {
        return;
    }

    size_t line_limit {Settings::max_line_length};
    if (comment[word_start] != '#')
    {
        stream << "# ";
        line_limit -= 2;
    }

    if (comment.length() - word_start <= line_limit)
    {
        stream << comment.substr(word_start) << '\n';
        return;
    }

    auto last_space {comment.find_last_of(' ', line_limit)};
    if (last_space == std::string_view::npos)
    {
        stream << comment.substr(word_start, line_limit - word_start) << '\n';
        m_add_comment(stream, comment.substr(line_limit));
    }
    else
    {
        auto last_char {comment.find_last_not_of(' ', last_space)};
        stream << comment.substr(word_start, last_char + 1) << '\n';
        m_add_comment(stream, comment.substr(last_space));
    }
}

void add_header_comment(std::ostream& stream, std::string_view comment)
{
    size_t word_start {comment.find_first_not_of(' ')};
    if (word_start == std::string_view::npos)
    {
        return;
    }

    size_t line_limit {Settings::max_line_length};
    if (comment[word_start] != '#')
    {
        stream << "# ";
        line_limit -= 2;
    }

    if (comment.length() - word_start <= line_limit)
    {
        stream << comment.substr(word_start) << '\n';
        return;
    }

    auto last_space {comment.find_last_of(' ', line_limit)};
    if (last_space == std::string_view::npos)
    {
        stream << comment.substr(word_start, line_limit - word_start) << '\n';
        m_add_comment(stream, comment.substr(line_limit));
    }
    else
    {
        auto last_char {comment.find_last_not_of(' ', last_space)};
        stream << comment.substr(word_start, last_char + 1) << '\n';
        m_add_comment(stream, comment.substr(last_space));
    }
}

[[maybe_unused]]
void write_header(std::ostream& stream, const PNMImageHeader& header)
{
    stream << PNMImageInfo::type_names[std::to_underlying(header.info.type)]
           << '\n';

    add_header_comment(stream, header.comments.main_comment);

    stream << header.info.width;
    if (header.comments.width_comment.empty())
    {
        stream << ' ';
    }
    else
    {
        stream << '\n';
        add_header_comment(stream, header.comments.width_comment);
    }

    stream << header.info.height << '\n';
    add_header_comment(stream, header.comments.height_comment);

    if (!PNMImageInfo::is_blackwhite(header.info.type))
    {
        stream << header.info.max_value << '\n';
        add_header_comment(stream, header.comments.max_value_comment);
    }
}

[[maybe_unused]]
void write_header_old(std::ostream& stream, PNMImageHeader header)
// std::ostream& stream, PNMHeader::Type type, std::string_view comment)
{
    stream << PNMImageInfo::type_names[std::to_underlying(header.info.type)]
           << '\n';

    FileOperations::m_add_comment(stream, header.comments.main_comment);

    stream << header.info.width << ' ' << header.info.height << '\n';

    if (!PNMImageInfo::is_blackwhite(header.info.type))
    {
        stream << header.info.max_value << '\n';
    }
    stream << PNMImageInfo::type_names[std::to_underlying(header.info.type)]
           << '\n';

    // m_add_comment(stream, comment);

    // stream << width() << ' ' << height() << '\n';

    // if (!PNMHeader::is_blackwhite(type))
    // {
    //     stream << max_value() << '\n';
    // }
}

template <pnm_suitable T>
void write_to_file(const PNMImageTemplated<T> image,
    const char* const file_path, std::ios_base::openmode open_mode,
    const HeaderComments& comments)
//    std::string_view comment)
{
    if ((open_mode & std::ios_base::app)
        && !(open_mode & std::ios_base::binary))
    {
        std::cout << "Appending images to one file is allowed only in "
                     "binary mode: opening as binary.\n";
        open_mode |= std::ios_base::binary;
    }
    std::ofstream file_stream {file_path, open_mode};

    if (!file_stream.good())
    {
        std::cerr << "Failed to open file " << file_path << " for writing.\n";
        return;
    }

    bool is_raw {(open_mode & std::ios_base::binary) != 0};

    PNMImageInfo info {image, is_raw};
    // PNMImageInfo info {*this, is_raw ? raw_type() : plain_type()};
    // HeaderComments comments {};
    // comments.main_comment = comment;
    PNMImageHeader header {};
    header.info = info;
    header.comments = comments;
    FileOperations::write_header(file_stream, header);

    if (is_raw)
    {
        // image.m_write_header(
        //     file_stream, pnm_image_traits<T>::raw_type(), comment);
        write_raster_raw(file_stream, image);
    }
    else
    {
        // image.m_write_header(
        //     file_stream, pnm_image_traits<T>::plain_type(), comment);
        write_raster_plain(file_stream, image);
    }
    file_stream << '\n';
}

} // namespace FileOperations
} // namespace

PNMImageHeader::PNMImageHeader(std::istream& stream)
{
    stream >> std::skipws;

    std::string current_comment {};
    auto read_lambda {
        [&stream, &current_comment](auto& value, std::string_view error_message)
        {
            if (!(stream >> value))
            {
                std::cerr << error_message << "\n";
                return false;
            }
            else
            {
                current_comment
                    = FileOperations::skip_whitespace_and_comment(stream);
                return true;
            }
        }};

    std::string format_id {};
    if (!read_lambda(format_id, "Failed to read id info from PNM header."))
    {
        return;
    }

    const std::string main_comment {current_comment};

    size_type width {};
    if (!read_lambda(
            width, format_id + ": failed to read width info from PNM header."))
    {
        return;
    }
    const std::string width_comment {current_comment};

    size_type height {};
    if (!read_lambda(height,
            format_id + ": failed to read height info from PNM header."))
    {
        return;
    }
    const std::string height_comment {current_comment};

    const auto type {PNMImageInfo::type_from_string(format_id).value_or(
        PNMImageInfo::Type::max_type)};
    size_type max_value {};
    std::string max_value_comment {};
    if (!PNMImageInfo::is_blackwhite(type))
    {
        if (!read_lambda(max_value,
                format_id + ": failed to read max value info from PNM header."))
        {
            return;
        }
        max_value_comment = current_comment;
    }
    else
    {
        max_value = 1;
    }

    // PNMImageInfo image_info {};
    info.type = type;
    info.width = width;
    info.height = height;
    info.max_value = max_value;

    auto copy_comment_lambda {[](std::string& target, std::string_view comment)
        {
            if (!comment.empty())
            {
                target = comment;
            }
        }};
    copy_comment_lambda(comments.main_comment, main_comment);
    copy_comment_lambda(comments.width_comment, width_comment);
    copy_comment_lambda(comments.height_comment, height_comment);
    copy_comment_lambda(comments.max_value_comment, max_value_comment);

    // m_type = type;
    // m_width = width;
    // m_height = height;
    // m_max_value = max_value;

    std::cout << "Header: "
              << PNMImageInfo::type_names[std::to_underlying(info.type)]
              << "; width = " << width << "; height = " << height
              << "; max = " << max_value << "\n";
    auto show_comment_lambda {
        [](std::string_view comment_name, std::string_view comment)
        {
            if (!comment.empty())
            {
                std::cout << comment_name << ": " << comment << "\n";
            }
        }};

    show_comment_lambda("main comment", comments.main_comment);
    show_comment_lambda("width comment", comments.width_comment);
    show_comment_lambda("height comment", comments.height_comment);
    show_comment_lambda("max value comment", comments.max_value_comment);
}

#if 0
PNMHeader::PNMHeader(std::istream& stream)
{
    stream >> std::skipws;

    auto read_lambda {[&stream](auto& value, std::string_view error_message)
        {
            if (!(stream >> value))
            {
                std::cerr << error_message << "\n";
                return false;
            }
            else
            {
                FileOperations::skip_whitespace_and_comment(stream);
                return true;
            }
        }};

    std::string format_id {};
    if (!read_lambda(format_id, "Failed to read id info from PNM header."))
    {
        return;
    }

    size_type width {};
    if (!read_lambda(
            width, format_id + ": failed to read width info from PNM header."))
    {
        return;
    }

    size_type height {};
    if (!read_lambda(height,
            format_id + ": failed to read height info from PNM header."))
    {
        return;
    }

    auto type {type_from_string(format_id).value_or(PNMHeader::Type::max_type)};
    size_type max_value {};
    if (!is_blackwhite(type))
    {
        if (!read_lambda(max_value,
                format_id + ": failed to read max value info from PNM header."))
        {
            return;
        }
    }
    else
    {
        max_value = 1;
    }

    m_type = type;
    m_width = width;
    m_height = height;
    m_max_value = max_value;

    std::cout << "Header: " << PNMHeader::type_names[std::to_underlying(m_type)]
              << "; width = " << width << "; height = " << height
              << "; max = " << max_value << "\n";
}
#endif

template <pnm_suitable T>
PNMImageTemplated<T>::PNMImageTemplated(
    size_type width, size_type height, size_type max_value)
    : MatrixTemplated<value_type> {height, width},
      m_max_value {std::min(max_value, Settings::absolute_max_value)}
{
}

template <pnm_suitable T>
PNMImageTemplated<T>::PNMImageTemplated(size_type width, size_type height)
    : PNMImageTemplated {
          width, height, pnm_image_traits<T>::default_max_value()}
{
}

template <pnm_suitable T>
void PNMImageTemplated<T>::resize(size_type new_width, size_type new_height)
{
    try
    {
        PNMImageTemplated image {new_width, new_height, this->max_value()};
        const auto scale_row {height() / static_cast<double>(new_height)};
        const auto scale_col {width() / static_cast<double>(new_width)};

        for (size_type row {0}; row < new_height; ++row)
        {
            const auto row_index {static_cast<size_type>(row * scale_row)};
            for (size_type col {0}; col < new_width; ++col)
            {
                image[row, col] = (*this)[row_index,
                    static_cast<size_type>(col * scale_col)];
            }
        }

        *this = std::move(image);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to resize image for size (" << new_width << "x"
                  << new_height << "): " << e.what() << "\n";
        return;
    }
}

template <pnm_suitable T>
void PNMImageTemplated<T>::scale(double scale_value)
{
    resize(static_cast<size_type>(width() * scale_value),
        static_cast<size_type>(height() * scale_value));
}

template <pnm_suitable T>
void PNMImageTemplated<T>::read_from(
    const char* const file_path, std::ios_base::openmode open_mode)
{
    try
    {
        std::ifstream file_stream {file_path, open_mode};
        if (!file_stream.good())
        {
            std::cerr << "Failed to open file " << file_path
                      << " for reading.\n";
            return;
        }

        std::string format_id {};
        file_stream >> format_id;

        const auto type {PNMImageInfo::type_from_string(format_id)};
        if (!type)
        {
            std::cerr << "Wrong image header id.\n";
            return;
        }

        const bool is_plain {type.value() == pnm_image_traits<T>::plain_type()};

        if (open_mode & std::ios_base::binary)
        {
            if (is_plain
                && !FileOperations::reopen_file(
                    file_stream, file_path, std::ios_base::binary))
            {
                return;
            }
        }
        else if (!is_plain
                 && !FileOperations::reopen_file(file_stream, file_path))
        {
            return;
        }

        // Set at the beginning of filestream (doesn't change if the stream was
        // not reopen).
        file_stream.clear();
        file_stream.seekg(0);

        const PNMImageHeader header {file_stream};

        try
        {
            *this = std::move(PNMImageTemplated {
                header.info.width, header.info.height, header.info.max_value});
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create image of size (" << header.info.width
                      << "x" << header.info.height << "): " << e.what() << "\n";
            return;
        }

        if (is_plain)
        {
            FileOperations::read_raster_plain(file_stream, *this);
        }
        else
        {
            FileOperations::read_raster_raw(file_stream, *this);
        }
    }
    catch (const std::ios_base::failure& failure)
    {
        std::cerr << "PNMImage: failed to read from file " << file_path
                  << "; what(): " << failure.what() << "\n";
    }
}

template <pnm_suitable T>
void PNMImageTemplated<T>::write_to(const char* const file_path,
    const HeaderComments& comments, std::ios_base::openmode open_mode) const
{
    try
    {
        FileOperations::write_to_file(*this, file_path, open_mode, comments);
    }
    catch (const std::ios_base::failure& failure)
    {
        std::cerr << "Failed to write to image file " << file_path
                  << "; what(): " << failure.what() << "\n";
    }
}

template <pnm_suitable T>
void PNMImageTemplated<T>::write_to(const char* const file_path,
    std::ios_base::openmode open_mode, const HeaderComments& comments) const
{
    write_to(file_path, comments, open_mode);
}

#if 1
template <pnm_suitable T>
void PNMImageTemplated<T>::write_to(const char* const file_path,
    std::ios_base::openmode open_mode, std::string_view comment) const
{
    write_to(file_path, HeaderComments {comment}, open_mode);
}

template <pnm_suitable T>
void PNMImageTemplated<T>::write_to(const char* const file_path,
    std::string_view comment, std::ios_base::openmode open_mode) const
{
    write_to(file_path, open_mode, comment);
}
#endif

// Instantiate concrete classes.
template class PNMImageTemplated<BlackWhiteColor>;
template class PNMImageTemplated<GrayScaleColor>;
template class PNMImageTemplated<RGBColor>;

std::unique_ptr<IPNMImage> make_image_ptr(const char* const file_path)
{
    try
    {
        std::ifstream file_stream {file_path, std::ios::binary};
        if (!file_stream.good())
        {
            std::cerr << "Failed to open file " << file_path
                      << " for reading.\n";
            return nullptr;
        }

        std::string format_id {};
        file_stream >> format_id;

        const auto type {PNMImageInfo::type_from_string(format_id)};
        if (!type)
        {
            std::cerr << "Wrong image header id.\n";
            return nullptr;
        }

        std::unique_ptr<IPNMImage> image_ptr {nullptr};
        if (PNMImageInfo::is_blackwhite(type.value()))
        {
            image_ptr = std::make_unique<PBMImage>();
        }
        else if (PNMImageInfo::is_grayscale(type.value()))
        {
            image_ptr = std::make_unique<PGMImage>();
        }
        else if (PNMImageInfo::is_rgb(type.value()))
        {
            image_ptr = std::make_unique<PPMImage>();
        }

        image_ptr->read_from(file_path);

        return image_ptr;
    }
    catch (const std::ios_base::failure& failure)
    {
        std::cerr << "PNMImage: failed to read from file " << file_path
                  << "; what(): " << failure.what() << "\n";
        return nullptr;
    }
}

} // namespace PNM_Format

} // namespace Marvin
