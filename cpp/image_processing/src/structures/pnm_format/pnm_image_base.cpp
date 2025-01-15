#include "structures/pnm_format/pnm_image_base.h"

#include <iomanip>
#include <algorithm>
#include <utility>

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

/**
 * @brief Tools to operate with types of PNM files.
 */
namespace TypeTools
{
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
constexpr std::array<std::string_view, std::to_underlying(Type::max_type)>
    type_names {"P1", "P2", "P3", "P4", "P5", "P6"};

/**
 * @brief Convert string to header type.
 *
 * @param[in] format_id String representing header id.
 * @return constexpr std::optional<Type> Type representing image format or
 * nullopt for illegal format id.
 */
constexpr std::optional<Type> type_from_string(std::string_view format_id)
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

/**
 * @brief Check whether the type represent a black and white image.
 *
 * @param[in] type Type to check.
 * @return true Image is black and white.
 * @return false Image is not black and white.
 */
constexpr bool is_blackwhite(Type type)
{
    return (type == Type::P1) || (type == Type::P4);
}

/**
 * @brief Check whether the type represent a grayscale image.
 *
 * @param[in] type Type to check.
 * @return true Image is grayscale.
 * @return false Image is not grayscale.
 */
constexpr bool is_grayscale(Type type)
{
    return (type == Type::P2) || (type == Type::P5);
}

/**
 * @brief Check whether the type represent an rgb image.
 *
 * @param[in] type Type to check.
 * @return true Image is rgb.
 * @return false Image is not rgb.
 */
constexpr bool is_rgb(Type type)
{
    return (type == Type::P3) || (type == Type::P6);
}

/**
 * @brief Check whether the type represent plain (text) file.
 *
 * @param[in] type Type to check.
 * @return true File is plain.
 * @return false File is not plain.
 */
constexpr bool is_plain(Type type)
{
    return (type == Type::P1) || (type == Type::P2) || (type == Type::P3);
}

/**
 * @brief Check whether the type represent raw (binary) file.
 *
 * @param[in] type Type to check.
 * @return true File is raw.
 * @return false File is not raw.
 */
constexpr bool is_raw(Type type)
{
    return (type == Type::P4) || (type == Type::P5) || (type == Type::P6);
}
} // namespace TypeTools

/**
 * @brief Traits for PNM images, containing type specific qualities. For
 * example, plain and raw type identifiers.
 *
 * @tparam T One of PNM suitable types (BlackWhiteColor, GrayScaleColor,
 * RGBColor).
 */
template <pnm_suitable T>
class pnm_image_traits;

/**
 * @brief Traits for PBM (Black and White) images.
 *
 * @tparam BlackWhiteColor
 */
template <>
class pnm_image_traits<BlackWhiteColor>
{
  public:
    using size_type = PNMImageTemplated<BlackWhiteColor>::size_type;
    using type_type = TypeTools::Type;
    /**
     * @brief Default max value for black and white images.
     *
     * @return constexpr size_type Max value for a pixel.
     */
    static constexpr size_type default_max_value() { return 1; }

    /**
     * @brief Number of channels to represent a pixel's color.
     *
     * @return constexpr size_type Channel count.
     */
    static constexpr size_type channel_count() { return 1; }

    /**
     * @brief Type representing plain (text) files.
     *
     * @return constexpr type_type File type.
     */
    static constexpr type_type plain_type() { return TypeTools::Type::P1; }

    /**
     * @brief Type representing raw (binary) files.
     *
     * @return constexpr type_type File type.
     */
    static constexpr type_type raw_type() { return TypeTools::Type::P4; }
};

/**
 * @brief Traits for PGM (GrayScale) images.
 *
 * @tparam GrayScaleColor
 */
template <>
class pnm_image_traits<GrayScaleColor>
{
  public:
    using size_type = PNMImageTemplated<GrayScaleColor>::size_type;
    using type_type = TypeTools::Type;
    /**
     * @brief Default max value for grayscale images.
     *
     * @return constexpr size_type Max value for a pixel.
     */
    static constexpr size_type default_max_value() { return 255; }

    /**
     * @brief Number of channels to represent a pixel's color.
     *
     * @return constexpr size_type Channel count.
     */
    static constexpr size_type channel_count() { return 1; }

    /**
     * @brief Type representing plain (text) files.
     *
     * @return constexpr type_type File type.
     */
    static constexpr type_type plain_type() { return TypeTools::Type::P2; }

    /**
     * @brief Type representing raw (binary) files.
     *
     * @return constexpr type_type File type.
     */
    static constexpr type_type raw_type() { return TypeTools::Type::P5; }
};

/**
 * @brief Traits for PPM (RGB) images.
 *
 * @tparam RGBColor
 */
template <>
class pnm_image_traits<RGBColor>
{
  public:
    using size_type = PNMImageTemplated<RGBColor>::size_type;
    using type_type = TypeTools::Type;
    /**
     * @brief Default max value for a single channel in RGB images.
     *
     * @return constexpr size_type Max value for pixel's channel.
     */
    static constexpr size_type default_max_value() { return 255; }

    /**
     * @brief Number of channels to represent a pixel's color.
     *
     * @return constexpr size_type Channel count.
     */
    static constexpr size_type channel_count() { return 3; }

    /**
     * @brief Type representing plain (text) files.
     *
     * @return constexpr type_type File type.
     */
    static constexpr type_type plain_type() { return TypeTools::Type::P3; }

    /**
     * @brief Type representing raw (binary) files.
     *
     * @return constexpr type_type File type.
     */
    static constexpr type_type raw_type() { return TypeTools::Type::P6; }
};

/**
 * @brief Structure for a pnm image header, containing the image info and header
 * comment.
 */
class PNMImageHeader
{
public:
    using size_type = int;

    /***************************************************************************
     * Constructors section                                                    *
     **************************************************************************/

    /**
     * @brief Default constructor.
     */
    PNMImageHeader() = default;

    /**
     * @brief Construct a header from a type, image and optional comment.
     *
     * @tparam T Image pixel type.
     * @param[in] header_type Type of header file.
     * @param[in] image Image to get information from.
     * @param[in] comment Header comment.
     */
    template <pnm_suitable T>
    PNMImageHeader(TypeTools::Type header_type,
        const PNMImageTemplated<T>& image,
        std::string_view comment = std::string_view {});

    /**
     * @brief Construct a header from an image and optional comment. The first
     * value is a flag for raw/plain file type.
     *
     * @tparam T Image pixel type.
     * @param[in] is_raw Whether the file is raw (binary) or plain (text).
     * @param[in] image Image to get information from.
     * @param[in] comment Header comment.
     */
    template <pnm_suitable T>
    PNMImageHeader(bool is_raw, const PNMImageTemplated<T>& image,
        std::string_view comment = std::string_view {});

    /**
     * @brief Construct a header from a stream.
     *
     * @param[in] stream Stream to read from.
     */
    PNMImageHeader(std::istream& stream);

    /***************************************************************************
     * End of Constructors section                                             *
     **************************************************************************/

    /***************************************************************************
     * Getters section                                                         *
     **************************************************************************/

    constexpr TypeTools::Type type() const { return m_type; }
    constexpr size_type width() const { return m_width; }
    constexpr size_type height() const { return m_height; }
    constexpr size_type max_value() const { return m_max_value; }
    std::string_view comment() const { return m_comment; }

    /***************************************************************************
     * End of Getters section                                                  *
     **************************************************************************/

    void print() const
    {
        std::cout << "Header: "
                  << TypeTools::type_names[std::to_underlying(m_type)]
                  << "; width = " << m_width << "; height = " << m_height
                  << "; max = " << m_max_value << "\n";
        if (!m_comment.empty())
        {
            std::cout << "Comment: " << m_comment << "\n";
        }
    }

  private:
    TypeTools::Type m_type {};
    size_type m_width {};
    size_type m_height {};
    size_type m_max_value {};
    std::string m_comment {};
};

/**
 * @brief The idea is to place most of the functions related to read/write
 * operations for images into a separate place.
 */
namespace FileOperations
{

/**
 * @brief Skip whitespace and comments in a stream, collecting comments into a
 * string.
 *
 * @param[in] stream Stream to process (typically, a file stream).
 * @return std::string String representing comments from a stream.
 */
std::string skip_whitespace_and_comment(std::istream& stream)
{
    std::string comment {};
    while (true)
    {
        // The hash ('#') symbol marks the beginning of comment in a PNM file.
        if (const auto c {stream.get()}; c == '#')
        {
            // Skip leading spaces.
            while (std::isspace(stream.peek()))
            {
                stream.get();
            }
            // The comment continues till the end of line.
            std::string line {};
            std::getline(stream, line);
            if (!line.empty())
            {
                // If the comment is spread along lines, keep adding them.
                comment += line + "\n";
            }
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

/**
 * @brief Attempt to close and open the file stream from a file path.
 *
 * @param[in] file_stream Stream to reopen.
 * @param[in] file_path Path to the file.
 * @param[in] open_mode Specify how to open the file.
 * @return true File stream was successfully reopened.
 * @return false Failed to reopen the stream.
 */
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

/**
 * @brief Retrive color values from a stream.
 *
 * @tparam T Color type.
 * @param[in] stream Stream to read from.
 * @param[in] image Image to write to.
 */
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

/**
 * @brief Read raw (binary) data from a stream. The methods vary depending on
 * the color type.
 *
 * @tparam T Color type.
 */
template <pnm_suitable T>
void read_raster_raw(std::istream&, PNMImageTemplated<T>&);

/**
 * @brief Read raw (binary) BlackWhiteColor data from a stream.
 *
 * @remarks Each row is Width bits, packed 8 to a byte, with don't care bits to
 * fill out the last byte in the row. Each bit represents a pixel: 1 is black, 0
 * is white. The order of the pixels is left to right. The order of their
 * storage within each file byte is most significant bit to least significant
 * bit. The order of the file bytes is from the beginning of the file toward the
 * end of the file. (Source: https://netpbm.sourceforge.net/doc/pbm.html)
 *
 * @tparam BlackWhiteColor.
 * @param[in] stream Stream to read from.
 * @param[in] image Image to write to.
 */
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
            // Retrieve each pixel from the packed byte (char, uint8) value.
            while (bit_count--)
            {
                // Extract most significant bits first.
                image[pixel_index++] = byte_value & 1 << --shift;
            }
        }};

    // Number of bytes in a row, reprsenting packed pixel values.
    const auto row_value_count {image.width() / 8};
    // Number of bits in the last pixel. Reason: ignoring extra bits.
    const int last_value_bit_count {static_cast<int>(image.width() % 8)};

    for (size_type row {0}; row < image.height(); ++row)
    {
        for (size_type i {0}; i < row_value_count; ++i)
        {
            // Each byte contains 8 bits.
            add_pixel_lambda(8);
        }

        // If there are extra bits, process only what's necessary.
        if (last_value_bit_count)
        {
            add_pixel_lambda(last_value_bit_count);
        }
    }
}

/**
 * @brief Helper function to retrive a two-byte grayscale value from a stream.
 *
 * @remarks The reason is to use this function not only for GrayScale images,
 * but also for RGB ones, because there are three channels (red, green and blue)
 * which essentially are grayscale.
 *
 * @param[in] stream Stream to read from.
 * @return int Value representing the grayscale color.
 */
int pnm_raw_gray_value_read(std::istream& stream)
{
    const auto first {stream.get()};
    const auto second {stream.get()};
    // The most significant byte is first.
    return (first << 8) | second;
}

/**
 * @brief Read raw (binary) GrayScaleColor data from a stream.
 *
 * @remarks Each gray value is a number from 0 through Maxval, with 0 being
 * black and Maxval being white. Each gray value is represented in pure binary
 * by either 1 or 2 bytes. If the Maxval is less than 256, it is 1 byte.
 * Otherwise, it is 2 bytes. The most significant byte is first.
 * (Source: https://netpbm.sourceforge.net/doc/pgm.html)
 *
 * @tparam GrayScaleColor.
 * @param[in] stream Stream to read from.
 * @param[in] image Image to write to.
 */
template <>
void read_raster_raw<GrayScaleColor>(
    std::istream& stream, PNMImageTemplated<GrayScaleColor>& image)
{
    // Values require two bytes per pixel.
    if (image.max_value() > 0xFF)
    {
        std::ranges::for_each(image.values(),
            [&stream](auto& p) { p = pnm_raw_gray_value_read(stream); });
    }
    else
    {
        std::ranges::for_each(
            image.values(), [&stream](auto& p) { p = stream.get(); });
    }
}

/**
 * @brief Read raw (binary) RGBColor data from a stream.
 *
 * @remarks Each pixel is a triplet of red, green, and blue samples, in that
 * order. Each sample is represented in pure binary by either 1 or 2 bytes. If
 * the Maxval is less than 256, it is 1 byte. Otherwise, it is 2 bytes. The most
 * significant byte is first.
 * (Source: https://netpbm.sourceforge.net/doc/ppm.html)
 *
 * @tparam RGBColor.
 * @param[in] stream Stream to read from.
 * @param[in] image Image to write to.
 */
template <>
void read_raster_raw<RGBColor>(
    std::istream& stream, PNMImageTemplated<RGBColor>& image)
{
    // Values require two bytes per pixel.
    if (image.max_value() > 0xFF)
    {
        std::ranges::for_each(image.values(),
            [&stream](auto& p)
            {
                const auto red {pnm_raw_gray_value_read(stream)};
                const auto green {pnm_raw_gray_value_read(stream)};
                const auto blue {pnm_raw_gray_value_read(stream)};
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

/**
 * @brief Helper function to place a pixel into the stream (reprsenting plain
 * text file) using width to align all items.
 *
 * @tparam T Color type.
 * @param[in] stream Stream to write to.
 * @param[in] color Color to write.
 * @param[in] item_width Width to align elements.
 */
template <pnm_suitable T>
void color_to_ostream_plain(
    std::ostream& stream, const T& color, int item_width)
{
    stream << std::setw(item_width) << color.value();
}

/**
 * @brief
 * @brief Specialization to place an RGB pixel into the stream since an RGB
 * color is repsented by three values.
 *
 * @tparam RGBColor.
 * @param[in] stream Stream to write to.
 * @param[in] color Color to write.
 * @param[in] item_width Width to align elements.
 */
template <>
void color_to_ostream_plain<RGBColor>(
    std::ostream& stream, const RGBColor& color, int item_width)
{
    stream << std::setw(item_width) << color.red() << ' '
           << std::setw(item_width) << color.green() << ' '
           << std::setw(item_width) << color.blue();
}

/**
 * @brief Write the raster data to a plain (text) stream (typicall, file
 * stream).
 *
 * @tparam T Color type.
 * @param[in] stream Stream to write to.
 * @param[in] image Image containing the raster data.
 */
template <pnm_suitable T>
void write_raster_plain(std::ostream& stream, const PNMImageTemplated<T>& image)
{
    using size_type = PNMImageTemplated<T>::size_type;

    auto number_width_lambda {[](size_type n)
        {
            size_type w {1};
            while (n /= 10)
            {
                ++w;
            }
            return w;
        }};

    stream << std::left;

    // Use the max value digit count to define the item width for alignment.
    const auto item_width {number_width_lambda(image.max_value())};

    // Important, because RGB images have three values to add per pixel, while
    // other image (GrayScale and BlackWhite) have only one value per pixel.
    const auto multiplier {pnm_image_traits<T>::channel_count()};

    // Initial space occupied by a pixel.
    const auto width_step {item_width * multiplier};
    // Step to the next pixel. The addition of multiplier represent the space
    // require for a whitespace character. Therefore, RGB color requires three
    // values and three spaces.
    const auto stride {width_step + multiplier};

    // Adjust line length limit in case the maximum space a row occupies is
    // smaller than the default max line length.
    const auto line_limit {
        std::min(Settings::max_line_length, image.width() * stride - 1)};

    size_type i {0};
    FileOperations::color_to_ostream_plain(stream, image[i], item_width);

    size_type current_width_pos {width_step};
    const auto n {image.width() * image.height()};
    while (++i < n)
    {
        current_width_pos += stride;
        if (current_width_pos > line_limit)
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

/**
 * @brief Function to write raw (binary) raster data to a stream. Vary for
 * different color types.
 *
 * @tparam T Color type.
 */
template <pnm_suitable T>
void write_raster_raw(std::ostream&, const PNMImageTemplated<T>&);

/**
 * @brief Function to write raw (binary) BlackWhite raster data to a stream.
 *
 * @remarks Each row is Width bits, packed 8 to a byte, with don't care bits to
 * fill out the last byte in the row. Each bit represents a pixel: 1 is black, 0
 * is white. The order of the pixels is left to right. The order of their
 * storage within each file byte is most significant bit to least significant
 * bit. The order of the file bytes is from the beginning of the file toward the
 * end of the file. (Source: https://netpbm.sourceforge.net/doc/pbm.html)

 * @tparam BlackWhiteColor.
 * @param[in] stream Stream to write to.
 * @param[in] image Image containing raster data.
 */
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
            // Pack pixel values into a byte.
            while (bit_count--)
            {
                // The most signficant bit is place first.
                byte_value |= image[pixel_index++].value() << --shift;
            }

            stream << byte_value;

            return col;
        }};

    // Number of bytes to pack the values into.
    const auto row_value_count {image.width() / 8};
    // Last bits to pack values into. The extra bits are to be ignored.
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
            // If there are extra bits, process only what's necessary.
            col = add_pixel_lambda(col, last_value_bit_count);
        }
    }
}

/**
 * @brief Helper function to place a two-byte GrayScale value into a stream.
 *
 * @param[in] stream Stream to write to.
 * @param[in] value Value to place into the stream.
 */
void pnm_raw_gray_value_write(std::ostream& stream, int value)
{
    // Most significant byte first.
    stream << static_cast<unsigned char>((value >> 8) & 0xFF)
           << static_cast<unsigned char>(value & 0xFF);
}

/**
 * @brief Function to write raw (binary) GrayScale raster data to a stream.
 *
 * @remarks Each gray value is a number from 0 through Maxval, with 0 being
 * black and Maxval being white. Each gray value is represented in pure binary
 * by either 1 or 2 bytes. If the Maxval is less than 256, it is 1 byte.
 * Otherwise, it is 2 bytes. The most significant byte is first.
 * (Source: https://netpbm.sourceforge.net/doc/pgm.html)
 *
 * @tparam GrayScaleColor.
 * @param[in] stream Stream to write to.
 * @param[in] image Image containing raster data.
 */
template <>
void write_raster_raw<GrayScaleColor>(
    std::ostream& stream, const PNMImageTemplated<GrayScaleColor>& image)
{
    // Values require two bytes per pixel.
    if (image.max_value() > 0xFF)
    {
        std::ranges::for_each(image.values(), [&stream](const auto& color)
            { pnm_raw_gray_value_write(stream, color.value()); });
    }
    else
    {
        std::ranges::for_each(image.values(), [&stream](const auto& color)
            { stream << static_cast<unsigned char>(color.value()); });
    }
}

/**
 * @brief Function to write raw (binary) RGB raster data to a stream.
 *
 * @remarks Each pixel is a triplet of red, green, and blue samples, in that
 * order. Each sample is represented in pure binary by either 1 or 2 bytes. If
 * the Maxval is less than 256, it is 1 byte. Otherwise, it is 2 bytes. The most
 * significant byte is first.
 * (Source: https://netpbm.sourceforge.net/doc/ppm.html)
 *
 * @tparam RGBColor.
 * @param[in] stream Stream to write to.
 * @param[in] image Image containing raster data.
 */
template <>
void write_raster_raw<RGBColor>(
    std::ostream& stream, const PNMImageTemplated<RGBColor>& image)
{
    // Values require two bytes per pixel.
    if (image.max_value() > 0xFF)
    {
        std::ranges::for_each(image.values(),
            [&stream](const auto& color)
            {
                pnm_raw_gray_value_write(stream, color.red());
                pnm_raw_gray_value_write(stream, color.green());
                pnm_raw_gray_value_write(stream, color.blue());
            });
    }
    else
    {
        std::ranges::for_each(image.values(),
            [&stream](const auto& color)
            {
                stream << static_cast<unsigned char>(color.red())
                       << static_cast<unsigned char>(color.green())
                       << static_cast<unsigned char>(color.blue());
            });
    }
}

/**
 * @brief Helper function to write a comment into a stream.
 *
 * @param[in] stream Stream to write to.
 * @param[in] comment Comment to add to the stream.
 */
void add_header_comment(std::ostream& stream, std::string_view comment)
{
    size_t word_start {comment.find_first_not_of(' ')};
    if (word_start == std::string_view::npos)
    {
        return;
    }

    size_t line_limit {Settings::max_line_length};
    // Comments in pnm files should begin with a hash symbol.
    if (comment[word_start] != '#')
    {
        stream << "# ";
        line_limit -= 2;
    }

    // Place the entire string if its length doesn't exceed the limit.
    if (comment.length() - word_start <= line_limit)
    {
        stream << comment.substr(word_start) << '\n';
        return;
    }

    auto last_space {comment.find_last_of(' ', line_limit)};
    if (last_space == std::string_view::npos)
    {
        // If there are no spaces within the limits (very long word - very
        // rare), then break the line inside the word, and process the rest of
        // the string.
        stream << comment.substr(word_start, line_limit - word_start) << '\n';
        add_header_comment(stream, comment.substr(line_limit));
    }
    else
    {
        // Break the line before the whitespace and process the rest of the
        // string.
        auto last_char {comment.find_last_not_of(' ', last_space)};
        stream << comment.substr(word_start, last_char + 1) << '\n';
        add_header_comment(stream, comment.substr(last_space));
    }
}

/**
 * @brief Add header to the stream.
 *
 * @param[in] stream Stream to write to.
 * @param[in] header Header to place into the stream.
 */
void write_header(std::ostream& stream, const PNMImageHeader& header)
{
    stream << TypeTools::type_names[std::to_underlying(header.type())] << '\n';

    add_header_comment(stream, header.comment());

    stream << header.width() << ' ' << header.height() << '\n';

    // BlackWhite image have no max value entity - their max value equals one.
    if (!TypeTools::is_blackwhite(header.type()))
    {
        stream << header.max_value() << '\n';
    }
}

/**
 * @brief Write an image to a file.
 *
 * @tparam T Underlying color type.
 * @param[in] image Image to write into the file.
 * @param[in] file_path Path to the file.
 * @param[in] open_mode Mode to open the file.
 * @param[in] comment Comment to add.
 */
template <pnm_suitable T>
void write_to_file(const PNMImageTemplated<T> image,
    const char* const file_path, std::ios_base::openmode open_mode,
    std::string_view comment)
{
    if ((open_mode & std::ios_base::app)
        && !(open_mode & std::ios_base::binary))
    {
        std::cout << "Info: File " << file_path
                  << ": appending images to one file is allowed only in "
                     "binary mode: opening as binary.\n";
        open_mode |= std::ios_base::binary;
    }
    std::ofstream file_stream {file_path, open_mode};

    if (!file_stream.good())
    {
        std::cerr << "Failed to open file " << file_path << " for writing.\n";
        return;
    }

    if (open_mode & std::ios_base::binary)
    {
        FileOperations::write_header(
            file_stream, {pnm_image_traits<T>::raw_type(), image, comment});
        write_raster_raw(file_stream, image);
    }
    else
    {
        FileOperations::write_header(
            file_stream, {pnm_image_traits<T>::plain_type(), image, comment});
        write_raster_plain(file_stream, image);
    }
    file_stream << '\n';
}

} // namespace FileOperations
} // namespace

/*******************************************************************************
 * Header's Constructor section.                                               *
 ******************************************************************************/

/**
 * @brief Construct a header from a type, image and optional comment.
 *
 * @tparam T Image pixel type.
 * @param[in] header_type Type of header file.
 * @param[in] image Image to get information from.
 * @param[in] comment Header comment.
 */
template <pnm_suitable T>
PNMImageHeader::PNMImageHeader(TypeTools::Type header_type,
    const PNMImageTemplated<T>& image, std::string_view comment)
    : m_type {header_type}, m_width {image.width()}, m_height {image.height()},
      m_max_value {image.max_value()}, m_comment {comment}
{
}

/**
 * @brief Construct a header from an image and optional comment. The first
 * value is a flag for raw/plain file type.
 *
 * @tparam T Image pixel type.
 * @param[in] is_raw Whether the file is raw (binary) or plain (text).
 * @param[in] image Image to get information from.
 * @param[in] comment Header comment.
 */
template <pnm_suitable T>
PNMImageHeader::PNMImageHeader(
    bool is_raw, const PNMImageTemplated<T>& image, std::string_view comment)
    : PNMImageHeader {is_raw ? pnm_image_traits<T>::raw_type()
                             : pnm_image_traits<T>::plain_type(),
          image, comment}
{
}

/**
 * @brief Construct a header from a stream.
 *
 * @param[in] stream Stream to read from.
 */
PNMImageHeader::PNMImageHeader(std::istream& stream)
{
    auto read_lambda {
        [&stream, this](auto& value, std::string_view error_message)
        {
            stream >> std::skipws;
            if (!(stream >> value))
            {
                std::cerr << error_message << "\n";
                return false;
            }
            else
            {
                const auto current_comment {
                    FileOperations::skip_whitespace_and_comment(stream)};
                if (!current_comment.empty())
                {
                    m_comment += current_comment;
                    // current_comment += current_comment;
                }
                return true;
            }
        }};

    std::string format_id {};
    if (!read_lambda(format_id, "Failed to read id from PNM header.")
        || !read_lambda(
            m_width, format_id + ": failed to read width from PNM header.")
        || !read_lambda(
            m_height, format_id + ": failed to read height from PNM header."))
    {
        return;
    }

    m_type = TypeTools::type_from_string(format_id).value_or(
        TypeTools::Type::max_type);

    if (!TypeTools::is_blackwhite(m_type))
    {
        if (!read_lambda(m_max_value,
                format_id + ": failed to read max value info from PNM header."))
        {
            return;
        }
    }
    else
    {
        m_max_value = 1;
    }

    //print();
}

/*******************************************************************************
 * End of  Header's Constructor section.                                       *
 ******************************************************************************/

/*******************************************************************************
 * PNMImageTemplated Constructors section.                                     *
 ******************************************************************************/

/**
 * @brief Initialize an image from width, height and max value.
 * 
 * @tparam T Color type.
 * @param[in] width Width of the image.
 * @param[in] height Height of the image.
 * @param[in] max_value Max value for each channel pixel value.
 */
template <pnm_suitable T>
PNMImageTemplated<T>::PNMImageTemplated(
    size_type width, size_type height, size_type max_value)
    : MatrixTemplated<value_type> {height, width},
      m_max_value {std::min(max_value, Settings::absolute_max_value)}
{
}

/**
 * @brief Initialize an image from width and height, using default max value.
 * 
 * @tparam T Color type.
 * @param[in] width Width of the image.
 * @param[in] height Height of the image.
 */
template <pnm_suitable T>
PNMImageTemplated<T>::PNMImageTemplated(size_type width, size_type height)
    : PNMImageTemplated {
          width, height, pnm_image_traits<T>::default_max_value()}
{
}

/**
 * @brief Construct an image from a file.
 * 
 * @tparam T Color type.
 * @param[in] file_path Path to the file to read from.
 * @param[in] open_mode Mode to open the file.
 */
template <pnm_suitable T>
PNMImageTemplated<T>::PNMImageTemplated(
    std::string_view file_path, std::ios_base::openmode open_mode)
{
    read_from(file_path.data(), open_mode);
}

/*******************************************************************************
 * End of PNMImageTemplated Constructors section.                              *
 ******************************************************************************/

/**
 * @brief Resize an image.
 * 
 * @tparam T Color type.
 * @param[in] new_width New width.
 * @param[in] new_height New height.
 */
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

/**
 * @brief Scale (resize) an image.
 * 
 * @tparam T Color type.
 * @param[in] scale_value Value to scale the image with.
 */
template <pnm_suitable T>
void PNMImageTemplated<T>::scale(double scale_value)
{
    if (scale_value > 0)
    {
        resize(static_cast<size_type>(width() * scale_value),
            static_cast<size_type>(height() * scale_value));
    }
}

/**
 * @brief Read an image from a file.
 * 
 * @tparam T Color type.
 * @param[in] file_path Path to the file.
 * @param[in] open_mode Mode to open the file.
 */
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

        const auto type {TypeTools::type_from_string(format_id)};
        if (!type)
        {
            std::cerr << "Wrong image header id.\n";
            return;
        }

        const bool is_plain {type.value() == pnm_image_traits<T>::plain_type()};

        // Reopen the file if it was open in the wrong mode.
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
                header.width(), header.height(), header.max_value()});
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create image of size (" << header.width()
                      << "x" << header.height() << "): " << e.what() << "\n";
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

/**
 * @brief Write an image to file.
 * 
 * @tparam T Color type.
 * @param[in] file_path Path to the file.
 * @param[in] comment Comment to add to the header.
 * @param[in] open_mode Mode to open the file.
 */
template <pnm_suitable T>
void PNMImageTemplated<T>::write_to(const char* const file_path,
    std::string_view comment, std::ios_base::openmode open_mode) const
{
    try
    {
        FileOperations::write_to_file(*this, file_path, open_mode, comment);
    }
    catch (const std::ios_base::failure& failure)
    {
        std::cerr << "Failed to write to image file " << file_path
                  << "; what(): " << failure.what() << "\n";
    }
}

/**
 * @brief Write an image to file.
 * 
 * @tparam T Color type.
 * @param[in] file_path Path to the file.
 * @param[in] open_mode Mode to open the file.
 * @param[in] comment Comment to add to the header.
 */
//template <pnm_suitable T>
//void PNMImageTemplated<T>::write_to(const char* const file_path,
//    std::ios_base::openmode open_mode, std::string_view comment) const
//{
//    write_to(file_path, comment, open_mode);
//}

// Instantiate concrete classes.
template class PNMImageTemplated<BlackWhiteColor>;
template class PNMImageTemplated<GrayScaleColor>;
template class PNMImageTemplated<RGBColor>;

/**
 * @brief Create an image pointer from a file.
 *
 * @param[in] file_path Path to the file.
 * @return std::unique_ptr<IPNMImage> Pointer to the created image.
 */
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

        const auto type {TypeTools::type_from_string(format_id)};
        if (!type)
        {
            std::cerr << "Wrong image header id.\n";
            return nullptr;
        }

        if (TypeTools::is_blackwhite(type.value()))
        {
            return std::make_unique<PBMImage>(file_path);
        }
        else if (TypeTools::is_grayscale(type.value()))
        {
            return std::make_unique<PGMImage>(file_path);
        }
        else if (TypeTools::is_rgb(type.value()))
        {
            return std::make_unique<PPMImage>(file_path);
        }
        else
        {
            return nullptr;
        }
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
