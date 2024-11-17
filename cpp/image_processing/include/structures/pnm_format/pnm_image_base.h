#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H

#include "structures/colors/rgb_color.h"
#include "structures/colors/grayscale_color.h"
#include "structures/colors/blackwhite_color.h"

#include <array>
#include <string_view>
#include <cstdint>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <exception>
#include <limits>
#include <iomanip>
#include <fstream>

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

static void skip_whitespace_and_comment(std::istream& stream)
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

        skip_whitespace_and_comment(stream);

        stream >> m_width;
        std::cout << "Width = " << m_width << "\n";

        skip_whitespace_and_comment(stream);

        stream >> m_height;
        std::cout << "Height = " << m_height << "\n";

        skip_whitespace_and_comment(stream);

        if (!is_blackwhite(m_type))
        {
            stream >> m_max_value;
        }
        else
        {
            m_max_value = 1;
        }
        std::cout << "Max value = " << m_max_value << "\n";

        skip_whitespace_and_comment(stream);
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

class IPNMImage
{
  protected:
    virtual void m_read_raster_data_raw(std::istream& stream) = 0;
    virtual void m_write_raster_data_raw(std::ostream& stream) const = 0;

  public:
    virtual ~IPNMImage() = default;

    virtual constexpr PNMHeader::Type plain_type() const = 0;
    virtual constexpr PNMHeader::Type raw_type() const = 0;

    virtual void read_from(const char* const file_path) = 0;
    virtual void write_to(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary,
        std::string_view comment = "")
        = 0;
};

template <class T>
class PNMImage_Common : public IPNMImage
{
  public:
    using value_type = T;
    using size_type = int;

  protected:
    PNMImage_Common() = default;

    PNMImage_Common(size_type width, size_type height, size_type max_value)
    try : m_pixels(width* height), m_width {width}, m_height {height},
        m_max_value {max_value}
    {
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to construct image of size(" << width << "x"
                  << height << "): " << e.what() << "\n";
        throw;
    }

  public:
    constexpr PNMHeader::Type plain_type() const override
    {
        return PNMHeader::Type::max_type;
    }

    constexpr PNMHeader::Type raw_type() const override
    {
        return PNMHeader::Type::max_type;
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

    const std::vector<value_type>& data() const
    {
        return m_pixels;
    }

    std::vector<value_type>& data()
    {
        return m_pixels;
    }

    T& operator[](size_type i)
    {
        return m_pixels[i];
    }

    const T& operator[](size_type i) const
    {
        return m_pixels[i];
    }

    T& operator[](size_type i, size_type j)
    {
        return operator[](i * width() + j);
    }

    const T& operator[](size_type i, size_type j) const
    {
        return operator[](i * width() + j);
    }

    void read_from(const char* const file_path) override
    {
        try
        {
            std::ifstream file_stream {file_path, std::ios::binary};
            if (!file_stream.good())
            {
                std::cerr << "Failed to open file " << file_path
                          << " for reading.\n";
                return;
            }

            std::string format_id {};
            file_stream >> format_id;

            const auto type {PNMHeader::type_from_string(format_id)};

            const bool is_plain {type == plain_type()};
            if (!is_plain && (type != raw_type()))
            {
                std::cerr << "Wrong image header id.\n";
                return;
            }

            file_stream.close();
            file_stream.open(file_path,
                is_plain ? std::ios_base::openmode {} : std::ios_base::binary);

            if (!file_stream.good())
            {
                std::cerr << "Failed to reopen file " << file_path
                          << " for reading.\n";
                return;
            }

            const PNMHeader header {file_stream};
            if (!m_try_init_from_header(header))
            {
                return;
            }

            if (is_plain)
            {
                m_read_raster_data_plain(file_stream);
            }
            else
            {
                m_read_raster_data_raw(file_stream);
            }
        }
        catch (const std::ios_base::failure& failure)
        {
            std::cerr << "PPMImage: failed to read from file " << file_path
                      << "; what(): " << failure.what() << "\n";
        }
    }

    void write_to(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary,
        std::string_view comment = "")
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
                m_write_header(file_stream, raw_type(), comment);
                m_write_raster_data_raw(file_stream);
            }
            else
            {
                m_write_header(file_stream, plain_type(), comment);
                m_write_raster_data_plain(file_stream);
            }
        }
        catch (const std::ios_base::failure& failure)
        {
            std::cerr << "Failed to write to image file " << file_path
                      << "; what(): " << failure.what() << "\n";
        }
    }

    void write_to(const char* const file_path, std::string_view comment,
        std::ios_base::openmode open_mode = std::ios_base::binary)
    {
        write_to(file_path, open_mode, comment);
    }

  protected:
    virtual constexpr int channel_count() const
    {
        return 1;
    }

    virtual void value_to_ostream_plain(std::ostream& stream,
        const value_type& color, size_type item_width) const
    {
        stream << std::setw(item_width) << color;
    }

  private:
    std::vector<value_type> m_pixels {};

    size_type m_width {};
    size_type m_height {};
    size_type m_max_value {};

    void m_read_raster_data_plain(std::istream& stream)
    {
        for (auto& color : m_pixels)
        {
            stream >> color;
        }
    }

    void m_write_raster_data_plain(std::ostream& stream) const
    {
        stream << std::left;
        const auto item_width {
            static_cast<size_type>(std::to_string(max_value()).length())};
        const auto multiplier {channel_count()};
        const auto width_step {item_width * multiplier};
        //const auto stride {multiplier * (item_width + 1)};
        const auto stride {width_step + multiplier};
        const auto line_limit {std::min(70, width() * stride - 1)};

        size_type i {0};
        value_to_ostream_plain(stream, operator[](i), item_width);

        size_type current_width_pos {width_step};
        const auto n {width() * height()};
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

            value_to_ostream_plain(stream, operator[](i), item_width);
        }
    }

    bool m_try_init_from_header(const PNMHeader& header)
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

        // m_type = header.type();
        m_width = header.width();
        m_height = header.height();
        m_max_value = header.max_value();

        return true;
    }

    void m_write_header(std::ostream& stream, PNMHeader::Type type,
        std::string_view comment) const
    {
        stream << PNMHeader::type_names[std::to_underlying(type)] << '\n';
        if (!comment.empty())
        {
            stream << "# " << comment << '\n';
        }
        stream << m_width << ' ' << m_height << '\n';

        if (!PNMHeader::is_blackwhite(type))
        {
            stream << m_max_value << '\n';
        }
    }

};

class RGBImage : public PNMImage_Common<RGBColor>
{
  public:
    RGBImage() = default;
    RGBImage(size_type width, size_type height, size_type max_value = 255)
        : PNMImage_Common<value_type> {width, height, max_value}
    {
    }

    constexpr PNMHeader::Type plain_type() const override
    {
        return PNMHeader::Type::P3;
    }

    constexpr PNMHeader::Type raw_type() const override
    {
        return PNMHeader::Type::P6;
    }

    void read_from(const char* const file_path)
    {
        PNMImage_Common::read_from(file_path);
    }

  private:
    virtual constexpr int channel_count() const
    {
        return 3;
    }

    void value_to_ostream_plain(std::ostream& stream, const value_type& color,
        size_type item_width) const override
    {
        stream << std::setw(item_width) << color.red() << ' '
               << std::setw(item_width) << color.green() << ' '
               << std::setw(item_width) << color.blue();
    }

    void m_read_raster_data_raw(std::istream& stream) override
    {
        if (max_value() > 0xFF)
        {
            for (auto& p : data())
            {
                std::array<RGBColor::value_type, 3> values {};
                for (auto& v : values)
                {
                    const auto first {stream.get()};
                    const auto second {stream.get()};
                    v = (first << 8) | second;
                }
                p = values;
            }
        }
        else
        {
            for (auto& p : data())
            {
                std::array<RGBColor::value_type, 3> values {};
                for (auto& v : values)
                {
                    v = stream.get();
                }
                p = values;
            }
        }
    }

    void m_write_raster_data_raw(std::ostream& stream) const override
    {
        for (const auto& color : data())
        {
            // std::array<Marvin::RGBColor::value_type, 3> values {
            //     color.red(), color.green(), color.blue()};
            // for (auto v : values)
            for (auto v : {color.red(), color.green(), color.blue()})
            {
                if (max_value() > 0xFF)
                {
                    stream << static_cast<char>((v >> 8) & 0xFF);
                    stream << static_cast<char>(v & 0xFF);
                }
                else
                {
                    stream << static_cast<char>(v);
                }
            }
        }
    }
};

class GSImage : public PNMImage_Common<GrayScaleColor>
{
  public:
    GSImage() = default;
    GSImage(size_type width, size_type height, size_type max_value = 255)
        : PNMImage_Common<value_type> {width, height, max_value}
    {
    }

    constexpr PNMHeader::Type plain_type() const override
    {
        return PNMHeader::Type::P2;
    }

    constexpr PNMHeader::Type raw_type() const override
    {
        return PNMHeader::Type::P5;
    }

    void read_from(const char* const file_path)
    {
        PNMImage_Common::read_from(file_path);
    }

  private:
    void m_read_raster_data_raw(std::istream& stream) override
    {
        if (max_value() > 0xFF)
        {
            for (auto& p : data())
            {
                const auto first {stream.get()};
                const auto second {stream.get()};
                p = (first << 8) | second;
            }
        }
        else
        {
            for (auto& p : data())
            {
                p = stream.get();
            }
        }
    }

    void m_write_raster_data_raw(std::ostream& stream) const override
    {
        if (max_value() > 0xFF)
        {
            for (const auto& color : data())
            {
                stream << static_cast<unsigned char>(
                    (color.value() >> 8) & 0xFF);
                stream << static_cast<unsigned char>(color.value() & 0xFF);
            }
        }
        else
        {
            for (const auto& color : data())
            {
                stream << static_cast<unsigned char>(color.value());
            }
        }
    }
};

class BWImage : public PNMImage_Common<BlackWhiteColor>
{
  public:
    BWImage() = default;
    BWImage(size_type width, size_type height)
        : PNMImage_Common<value_type> {width, height, 1}
    {
    }

    constexpr PNMHeader::Type plain_type() const override
    {
        return PNMHeader::Type::P1;
    }

    constexpr PNMHeader::Type raw_type() const override
    {
        return PNMHeader::Type::P4;
    }

    void read_from(const char* const file_path)
    {
        PNMImage_Common::read_from(file_path);
    }

  private:
    void m_read_raster_data_raw(std::istream& stream) override
    {
        int pixel_index {0};
        const auto add_pixel_lambda {
            [&stream, &pixel_index, this](int bit_count)
            {
                const auto byte_value {stream.get()};
                auto shift {8};
                while (bit_count--)
                {
                    // Extract most significant bits first.
                    operator[](pixel_index++) = byte_value & 1 << --shift;
                }
            }};

        const auto row_value_count {width() / 8};
        const int last_value_bit_count {static_cast<int>(width() % 8)};

        for (size_type row {0}; row < height(); ++row)
        {
            for (size_type i {0}; i < row_value_count; ++i)
            {
                add_pixel_lambda(8);
            }

            if (last_value_bit_count != 0)
            {
                add_pixel_lambda(last_value_bit_count);
            }
        }
    }

    void m_write_raster_data_raw(std::ostream& stream) const override
    {
        size_type pixel_index {0};
        auto add_pixel_lambda {
            [&stream, this, &pixel_index](size_type col, int bit_count)
            {
                unsigned char byte_value {};
                unsigned char shift {8};
                while (bit_count--)
                {
                    // The most signficant bit is place first.
                    byte_value |= operator[](pixel_index++).value() << --shift;
                }

                stream << byte_value;

                return col;
            }};

        const auto row_value_count {width() / 8};
        const int last_value_bit_count {static_cast<int>(width() % 8)};

        for (size_type row {0}; row < height(); ++row)
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
};

std::unique_ptr<IPNMImage> read_from_file(const char* const file_path);


} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
