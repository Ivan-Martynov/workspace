#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H

#include "structures/colors/rgb_color.h"
#include "structures/colors/grayscale_color.h"
#include "structures/colors/blackwhite_color.h"

#include <array>
#include <string_view>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <exception>
#include <iomanip>
#include <fstream>
#include <optional>

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

    /**
     * @brief Array containing names representing header ids.
     *
     */
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

    PNMHeader(std::istream& stream);

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

    virtual void read_from(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary)
        = 0;
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

    void scale(size_type new_width, size_type new_height)
    {
        const auto pixel_count {new_width * new_height};
        std::vector<value_type> temp {};
        try
        {
            temp.resize(pixel_count);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create array of pixels for " << pixel_count
                      << " elements: " << e.what() << "\n";
            return;
        }

        const auto scale_row {height() / static_cast<double>(new_height)};
        const auto scale_col {width() / static_cast<double>(new_width)};

        for (size_type row {0}; row < new_height; ++row)
        {
            for (size_type col {0}; col < new_width; ++col)
            {
                temp[row * new_width + col]
                    = operator[](static_cast<size_type>(row * scale_row),
                        static_cast<size_type>(col * scale_col));
            }
        }

        m_pixels = std::move(temp);
        m_width = new_width;
        m_height = new_height;
    }

    void scale(double scale_value)
    {
        scale(static_cast<size_type>(m_width * scale_value),
            static_cast<size_type>(m_height * scale_value));
    }

    void read_from(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary) override
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

            const auto type {PNMHeader::type_from_string(format_id)};
            if (!type)
            {
                std::cerr << "Wrong image header id.\n";
                return;
            }

            const bool is_plain {type.value() == plain_type()};

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
            if ((open_mode & std::ios_base::app)
                && !(open_mode & std::ios_base::binary))
            {
                std::cout << "Appending images to one file is allowed only in "
                             "binary mode.\n";
                open_mode |= std::ios_base::binary;
            }
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
            file_stream << '\n';
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

    void m_add_comment(std::ostream& stream, std::string_view comment) const
    {
        size_t word_start {comment.find_first_not_of(' ')};
        if (word_start == std::string_view::npos)
        {
            return;
        }

        size_t line_limit {70};
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
            stream << comment.substr(word_start, line_limit - word_start)
                   << '\n';
            m_add_comment(stream, comment.substr(line_limit));
        }
        else
        {
            auto last_char {comment.find_last_not_of(' ', last_space)};
            stream << comment.substr(word_start, last_char + 1) << '\n';
            m_add_comment(stream, comment.substr(last_space));
        }
    }

    void m_write_header(std::ostream& stream, PNMHeader::Type type,
        std::string_view comment) const
    {
        //auto line_limit {70};
        stream << PNMHeader::type_names[std::to_underlying(type)] << '\n';

        m_add_comment(stream, comment);
        //if (!comment.empty())
        //{
        //    stream << "# " << comment << '\n';
        //}

        stream << m_width << ' ' << m_height << '\n';

        if (!PNMHeader::is_blackwhite(type))
        {
            stream << m_max_value << '\n';
        }
    }

};

class PPMImage : public PNMImage_Common<RGBColor>
{
  public:
    PPMImage() = default;
    PPMImage(size_type width, size_type height, size_type max_value = 255)
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

  private:
    constexpr int channel_count() const override
    {
        return 3;
    }

    void value_to_ostream_plain(std::ostream& stream, const value_type& color,
        size_type item_width) const override;
    void m_read_raster_data_raw(std::istream& stream) override;
    void m_write_raster_data_raw(std::ostream& stream) const override;
    
};

//using PGMImage = PNMImage_Common<GrayScaleColor>;
class PGMImage : public PNMImage_Common<GrayScaleColor>
{
  public:
    PGMImage() = default;
    PGMImage(size_type width, size_type height, size_type max_value = 255)
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

  private:
    void m_read_raster_data_raw(std::istream& stream) override;
    void m_write_raster_data_raw(std::ostream& stream) const override;
};

/**
 * @brief PBM (Portable Bit Map) image is a black-white image, each pixel
 * reprsented by 1 or 0.
 *
 * @remarks These remarks concern files representing PBM images:
 * - Identifiers (aka magic numbers) are P1 for plain and P4 for raw data types.
 * - For raw data type, values are packed to have 8 values per byte for
 * each row. If there are extra bits in the last byte in the row, then they are
 * ignored.
 * - For plain data type each line cannot be longer than 70 characters.
 * - Header doesn't contain max value - it is always 1.
 */
class PBMImage : public PNMImage_Common<BlackWhiteColor>
{
  public:
    PBMImage() = default;
    PBMImage(size_type width, size_type height)
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

  private:
    void m_read_raster_data_raw(std::istream& stream) override;
    void m_write_raster_data_raw(std::ostream& stream) const override;
};

std::unique_ptr<IPNMImage> read_from_file(const char* const file_path);

} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
