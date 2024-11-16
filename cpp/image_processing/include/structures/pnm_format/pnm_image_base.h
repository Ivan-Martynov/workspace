#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H

//#include "structures/colors/rgb_color.h"

#include <array>
#include <string_view>
#include <cstdint>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <exception>
#include <limits>

namespace Marvin
{

namespace PNM_Format
{

/**
 * @brief General class, responsible for reading/writing image files.
 *
 * PBM image is a binary image. Allowed values are only 0 and 1. Can be
 * represented using bool.
 *
 * PGM image is a grayscale image. Every pixel is represented by a value from 0
 * to max_value. Max value cannot be bigger than 65535 (2^16 - 1, that is 16
 * bits). If the max value is less than 256, then uint8_t can suffice, otherwise
 * uint16_t can be used. Alternatively, another type can be used, but has to be
 * handled properly.
 *
 * PPM image is an RGB image. Every pixel is represented by three values for
 * red, green and blue channels.
 * 
 * Every image file can be represented in a plain or raw format.
 *
 */

/* Write/read to file. Use format to identify how to write header.
try
    write_to(filepath, type, comment):
        - write_header(stream, type, comment)
        - write raster data(mode: raw or plain)
catch (std::ios_base::failure)
try
    read_from(filepath)
        - read_header(stream) => type, widht, height, (max_value?)
        - read raster data(mode: raw or plain)
catch (std::ios_base::failure)
     */
class PNMImageBase
{
  public:
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

    enum class Mode
    {
        plain,
        raw,
    };

    static constexpr std::array<std::string_view,
        std::to_underlying(Type::max_type)>
        type_names {"P1", "P2", "P3", "P4", "P5", "P6"};

    std::intmax_t width() const noexcept;

    std::intmax_t height() const noexcept;

    std::intmax_t max_value() const noexcept;

    bool is_valid_type(Type type) const noexcept;
    bool is_valid_format_id(std::string_view format_id) const;
    bool is_plain_id(Type type) const;
    static Type get_header_type(std::string_view format_id);

    /* Write to file. Use format to identify how to write header.
    write_to(filepath, type, comment):
        - write_header (stream, type, comment)
        - write raster data
     */
    void read_header(std::istream& stream);
    void write_header(
        std::ostream& stream, Type type, std::string_view comment = "") const;

  private:
    std::intmax_t m_width {};
    std::intmax_t m_height {};
    std::intmax_t m_max_value {};
};

static void skip_ws_and_comment(std::istream & stream)
{
    while (std::isspace(stream.peek()) || (stream.peek() == '#'))
    {
        if (stream.get() == '#')
        {
            stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

class PNMHeader
{
  public:
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

    static constexpr std::array<std::string_view,
        std::to_underlying(Type::max_type)>
        type_names {"P1", "P2", "P3", "P4", "P5", "P6"};

    static constexpr bool is_valid_id(std::string_view id)
    {
        for (auto name : type_names)
        {
            if (name == id)
            {
                return true;
            }
        }

        return false;
    }

    static Type type_from_string(std::string_view format_id)
    {
        for (size_t i {0}; i < PNMImageBase::type_names.size(); ++i)
        {
            if (PNMImageBase::type_names[i] == format_id)
            {
                return static_cast<Type>(i);
            }
        }

        return Type::max_type;
    }

    static bool is_blackwhite(Type type)
    {
        return (type == Type::P1) || (type == Type::P4);
    }

    static bool is_grayscale(Type type)
    {
        return (type == Type::P2) || (type == Type::P5);
    }

    static bool is_rgb(Type type)
    {
        return (type == Type::P3) || (type == Type::P6);
    }

    static bool is_plain(Type type)
    {
        return (type == Type::P1) || (type == Type::P2) || (type == Type::P3);
    }

    static bool is_raw(Type type)
    {
        return (type == Type::P4) || (type == Type::P5) || (type == Type::P6);
    }

    PNMHeader(std::istream& stream)
    {
        stream >> std::skipws;
        std::string format_id {};
        stream >> format_id;

        m_type = type_from_string(format_id);

        skip_ws_and_comment(stream);

        stream >> m_width;
        std::cout << "Width = " << m_width << "\n";

        skip_ws_and_comment(stream);

        stream >> m_height;
        std::cout << "Height = " << m_height << "\n";

        skip_ws_and_comment(stream);

        if (!is_blackwhite(m_type))
        {
            stream >> m_max_value;
        }
        else
        {
            m_max_value = 1;
        }
        std::cout << "Max value = " << m_max_value << "\n";

        skip_ws_and_comment(stream);
    }

    Type type() const noexcept
    {
        return m_type;
    }

    size_type width() const noexcept
    {
        return m_width;
    }

    size_type height() const noexcept
    {
        return m_height;
    }

    size_type max_value() const noexcept
    {
        return m_max_value;
    }

  private:
    PNMHeader::Type m_type {};

    size_type m_width {};
    size_type m_height {};
    size_type m_max_value {};
};

#if 0
template <class T>
class PNMImageBase_Templated
{
  public:
    using value_type = T;
    using size_type = int;

    static std::unique_ptr<PNMImageBase_Templated> read_from(
        const char* const file_path, std::ios::openmode open_mode)
    {
        std::unique_ptr<PNMImageBase_Templated> image_ptr {nullptr};
        return image_ptr;
    }

    bool try_init_from_header(const PNMHeader& header)
    {
        const auto pixel_count {header.width() * header.height()};
        if (static_cast<size_t>(pixel_count) >= m_pixels.max_size())
        {
            std::cerr << "Cannot allocate more than max ("
                      << m_pixels.max_size() << ") allowed elements.\n";
            return false;
        }

        try
        {
            m_pixels.resize(pixel_count);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to resize array of pixels for " << pixel_count
                      << " elements: " << e.what() << "\n";
        }

        m_type = header.type();
        m_width = header.width();
        m_height = header.height();
        m_max_value = header.max_value();

        return true;
    }

    static void read_from(const char* const file_path,
        PNMImageBase_Templated& image,
        std::ios_base::openmode open_mode = std::ios_base::binary)
    {
        std::ifstream file_stream {file_path, open_mode};
        if (!file_stream.good())
        {
            std::cerr << "Failed to open file " << file_path
                      << " for reading.\n";
            return;
        }

        std::string format_id {};
        stream >> format_id;
        const type {PNMHeader::type_from_string(format_id)};

        if (!image.is_valid_type(type))
        {
            std::cerr << "Not a valid header id.\n";
            return;
        }

        const bool is_plain {PNMHeader::is_plain(type)};

        // Reopen the file as text or binary depending on the format.
        file_stream.close();
        file_stream.open(file_path,
            is_plain ? std::ios_base::openmode {} : std::ios_base::binary);

        if (!file_stream.good())
        {
            std::cerr << "Failed to reopen file " << file_path
                      << " for reading.\n";
            return;
        }

        PNMHeader header {stream};
        if (!image.try_init_from_header(header))
        {
            return;
        }

        if (is_plain)
        {
            image.read_raster_data_plain(file_stream);
        }
        else
        {
            image.read_raster_data_raw(file_stream);
        }
    }

    void read_raster_data_plain(std::istream& stream)
    {
        for (auto& p : m_pixels)
        {
            m_pixels >> p;
        }
    }

    void read_raster_data_raw(std::istream& stream)
    {
        for (auto& p : m_pixels)
        {
            p = stream.get();
        }
    }

    void write_header(std::ostream& stream, PNMHeader::Type type,
        std::string_view comment) const
    {
        if (!is_valid_type(type))
        {
            std::cerr << "Invalid type.\n";
            return;
        }

        stream << PNMHeader::type_names[std::to_underlying(type)] << '\n';

        if (!comment.empty())
        {
            stream << "# " << comment << '\n';
        }
        stream << m_width << ' ' << m_height << '\n';

        if (!PNMHeader::is_blackwhite(m_type))
        {
            stream << m_max_value << '\n';
        }
    }

    void write_to(const char* const file_path, std::ios_base
                  : openmode open_mode, std::string_view comment = "")
    {
        try
        {
            std::ofstream file_stream {file_path, open_mode};

            if (!file_stream.good())
            {
                std::cerr << "Failed to open file " << file_path
                          << " for writing.\n";
                return;
            }

            if (open_mode & std::ios_base::binary)
            {
                write_header(file_stream, raw_type(), comment);
                write_raster_data_raw(file_stream);
            }
            else
            {
                write_header(file_stream, plain_type(), comment);
                write_raster_data_plain(file_stream);
            }
        }
        catch (const std::ios_base::failure& failure)
        {
            std::cerr << "Failed to write to file " << file_path
                      << "; what(): " << failure.what() << "\n";
        }
    }

    void write_raster_data_plain(std::ostream& stream)
    {
        constexpr auto line_limit {70};

        const auto w {std::to_string(m_max_value).length()};

        stream << std::left;
        int cw {0};
        for (const auto& color : m_pixels)
        {
            cw += 3 * w + 5;
            const bool next_line {cw >= line_limit};
            if (next_line)
            {
                stream << '\n';
                cw = 0;
            }
            stream = pixel_to_plain_file_stream(stream, color);
            //stream << std::setw(w) << color.red() << ' '
            //       << std::setw(w) << color.green() << ' ' << std::setw(w)
            //       << color.blue() << ' ';
        }
    }

    void write_raster_data_raw(std::ostream& stream)
    {
        for (const auto& color : m_pixels)
        {
            stream = pixel_to_file_stream(color);
        }
    }

    std::istream& pixel_from_plain_file_stream(std::istream& stream, T& pixel)
    {
        return stream >> pixel;
    }

    std::istream& pixel_from_raw_file_stream(std::istream& stream, T& pixel)
    {
        return stream >> pixel;
    }

    std::ostream& pixel_to_plain_file_stream(
        std::ostream& stream, const T& pixel) const
    {
        return stream << pixel;
    }

    std::ostream& pixel_to_raw_file_stream(
        std::ostream& stream, const T& pixel) const
    {
        return stream << pixel;
    }

    constexpr bool is_valid_type(PNMHeader::Type type) const
    {
        return type != PNMHeader::Type::max_type;
    }

    constexpr bool is_plain(PNMHeader::Type type) const
    {
        return PNMHeader::is_plain(type);
    }

    constexpr bool is_raw(PNMHeader::Type type) const
    {
        return PNMHeader::is_raw(type);
    }

    PNMHeader::Type plain_type()
    {
        return PNMHeader::Type::P3;
    }

    constexpr PNMHeader::Type raw_type() const
    {
        return PNMHeader::Type::P6;
    }

  private:
    PNMHeader::Type m_type {};
    size_type m_width {};
    size_type m_height {};
    size_type m_max_value {};
    std::vector<T> m_pixels {};
};

//template <>
//inline PNMHeader::Type PNMImageBase_Templated<RGBColor>::plain_type()
//{
//    return PNMHeader::Type::P3;
//};

class RGBImage : PNMImageBase_Templated<RGBColor>
{
};

#endif

class IPNMImage
{
  public:
    virtual ~IPNMImage() = default;

    virtual PNMHeader::Type plain_type() const = 0;
    virtual PNMHeader::Type raw_type() const = 0;

    virtual void read_raster_data_plain(std::istream& stream) = 0;
    virtual void read_raster_data_raw(std::istream& stream) = 0;

    virtual void write_raster_data_plain(std::ostream& stream) = 0;
    virtual void write_raster_data_raw(std::ostream& stream) = 0;
};

} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
