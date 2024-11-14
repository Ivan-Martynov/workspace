#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PGM_IMAGE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PGM_IMAGE_H_H

#include <cstdint>
#include <string_view>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <charconv>
#include <expected>

namespace Marvin
{

namespace PNM_Format
{

class PGM_Image
{
  public:
    // using value_type = std::uint8_t;
    using value_type = std::uint16_t;

    PGM_Image() = default;

    PGM_Image(
        std::intmax_t width, std::intmax_t height, value_type max_value)
        : m_data_ptr {new value_type[width * height]}, m_width {width},
          m_height {height}, m_max_value {max_value}
    {
    }

    PGM_Image(const PGM_Image& other) = delete;
    PGM_Image& operator=(const PGM_Image& other) = delete;

    PGM_Image(PGM_Image&& other) noexcept
        : m_data_ptr {other.m_data_ptr}, m_width {other.m_width},
          m_height {other.m_height}, m_max_value {other.m_max_value}
    {
        other.m_data_ptr = nullptr;
    }

    PGM_Image& operator=(PGM_Image&& other) noexcept
    {
        if (this != &other)
        {
            delete[] m_data_ptr;

            m_data_ptr = other.m_data_ptr;
            m_width = other.m_width;
            m_height = other.m_height;

            other.m_data_ptr = nullptr;
        }

        return *this;
    }

    ~PGM_Image()
    {
        delete[] m_data_ptr;
    }

    std::intmax_t width() const
    {
        return m_width;
    }
    std::intmax_t height() const
    {
        return m_height;
    }
    value_type max_value() const
    {
        return m_max_value;
    }

    value_type& operator[](std::intmax_t i)
    {
        return m_data_ptr[i];
    }

    value_type& operator[](std::intmax_t row, std::intmax_t column)
    {
        return operator[](row * m_width + column);
    }

    value_type* data()
    {
        return m_data_ptr;
    }

    static std::expected<std::intmax_t, std::errc> get_int_value(
        std::istream& stream)
    {
        std::string text {};
        while (true)
        {
            stream >> text;
            if (text[0] == '#')
            {
                stream.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
            {
                break;
            }
        }

        std::intmax_t value {};
        auto [ptr, error_code] {
            std::from_chars(text.data(), text.data() + text.size(), value)};
        if (error_code == std::errc())
        {
            return value;
        }
        else
        {
            return std::unexpected(error_code);
        }
    }

    static PGM_Image read_plain(std::istream& stream)
    {
        // constexpr int max_line_width {70};

        std::expected<std::intmax_t, std::errc> width {get_int_value(stream)};
        if (!width.has_value())
        {
            std::cerr << "Failed to extract width.\n";
            return PGM_Image {};
        }
        else if (*width < 0)
        {
            std::cerr << "Width should be non-negative.\n";
            return PGM_Image {};
        }

        std::expected<std::intmax_t, std::errc> height {get_int_value(stream)};
        if (!height.has_value())
        {
            std::cerr << "Failed to extract height.\n";
            return PGM_Image {};
        }
        else if (*height < 0)
        {
            std::cerr << "Height should be non-negative.\n";
            return PGM_Image {};
        }

        std::expected<std::intmax_t, std::errc> max_value {
            get_int_value(stream)};
        if (!max_value.has_value())
        {
            std::cerr << "Failed to extract max value.\n";
            return PGM_Image {};
        }
        else if (*max_value < 0)
        {
            std::cerr << "Max value should be non-negative.\n";
            return PGM_Image {};
        }
        else if (*max_value > 65535)
        {
            std::cerr << "Max value exceeds 65535.\n";
            return PGM_Image {};
        }

        if (!std::isspace(stream.get()))
        {
            std::cerr << "Raster data should be preceded by a whitespace "
                         "character.\n";
            return PGM_Image {};
        }

        PGM_Image image {*width, *height, static_cast<value_type>(*max_value)};

        std::cout << "Width = " << image.width()
                  << "; height = " << image.height()
                  << "; max = " << image.max_value() << "\n";

        for (std::intmax_t i {0}; i < image.width() * image.height(); ++i)
        {
            std::intmax_t value {};
            stream >> value;
            image[i] = value;
        }
        std::cout << "\n";

        return image;
    }

    static PGM_Image read_raw(std::istream& stream)
    {
        std::expected<std::intmax_t, std::errc> width {get_int_value(stream)};
        if (!width.has_value())
        {
            std::cerr << "Failed to extract width.\n";
            return PGM_Image {};
        }

        std::expected<std::intmax_t, std::errc> height {get_int_value(stream)};
        if (!height.has_value())
        {
            std::cerr << "Failed to extract height.\n";
            return PGM_Image {};
        }

        std::expected<std::intmax_t, std::errc> max_value {
            get_int_value(stream)};
        if (!max_value.has_value())
        {
            std::cerr << "Failed to extract max value.\n";
            return PGM_Image {};
        }

        if (!std::isspace(stream.get()))
        {
            std::cerr << "Raster data should be preceded by a whitespace "
                         "character.\n";
            return PGM_Image {};
        }

        PGM_Image image {*width, *height, static_cast<value_type>(*max_value)};

#if 0
        if (image.max_value() > 0xFF)
        {
            stream.read(reinterpret_cast<char*>(image.data()),
                image.width() * image.height() * 2);
            //for (std::intmax_t i {0}; i < image.width() * image.height(); ++i)
            //{
            //    const auto first {static_cast<unsigned char>(stream.get())};
            //    image[i] = (first << 8) | stream.get();
            //}
        }
        else
        {
            stream.read(reinterpret_cast<char*>(image.data()),
                image.width() * image.height());
        }
#else
        for (std::intmax_t i {0}; i < image.width() * image.height(); ++i)
        {
            if (image.max_value() > 0xFF)
            {
#if 0
                char values[2] {};
                stream.get(values, 2);
                image[i] = (values[1] << 8) | values[0];
#else
                const auto first {stream.get()};
                const auto second {stream.get()};
                image[i] = (first << 8) | second;
#endif
            }
            else
            {
                image[i] = stream.get();
            }
        }
#endif
        return image;
    }

    static PGM_Image read_from_file(
        std::string_view file_path, std::ios_base::openmode open_mode)
    {
        std::ifstream stream {std::string {file_path}, open_mode};

        std::string format_id {};
        stream >> format_id;

        if (format_id == "P2")
        {
            return read_plain(stream);
        }
        else if (format_id == "P5")
        {
            return read_raw(stream);
        }
        else
        {
            return {};
        }
    }

    void write_to_plain(std::ostream& stream)
    {
        stream << "P2\n"
               << m_width << ' ' << m_height << '\n'
               << m_max_value << '\n';
        int k {0};
        for (std::intmax_t i {0}; i < m_width * m_height; ++i)
        {
            stream << (*this)[i] << ' ';
            if (++k > 15)
            {
                stream << '\n';
                k = 0;
            }
        }

        std::cout << "\n";
    }

    void write_to_binary(std::ostream& stream)
    {
        stream << "P5\n"
               << std::to_string(m_width) << ' ' << std::to_string(m_height)
               << '\n'
               << std::to_string(m_max_value) << '\n';
        for (std::intmax_t i {0}; i < m_width * m_height; ++i)
        {
            value_type pixel_value {(*this)[i]};
            if (m_max_value > 255)
            {
                stream << static_cast<char>((pixel_value >> 8) & 0xFF);
                stream << static_cast<char>(pixel_value & 0xFF);
            }
            else
            {
                stream << static_cast<char>(pixel_value);
            }
        }

        std::cout << "\n";
    }

    void write_to(std::string_view file_path, std::ios_base::openmode open_mode)
    {
        std::ofstream stream {std::string {file_path}, open_mode};

        if (open_mode & std::ios_base::binary)
        {
            write_to_binary(stream);
        }
        else
        {
            write_to_plain(stream);
        }

        for (std::intmax_t row {0}; row < m_height; ++row)
        {
            for (std::intmax_t col {0}; col < m_width; ++col)
            {
                std::cout << std::setw(5)
                          << static_cast<std::intmax_t>((*this)[row, col])
                          << ' ';
            }
            std::cout << '\n';
        }
        std::cout << "\n";
    }

  private:
    value_type* m_data_ptr {};
    std::intmax_t m_width {};
    std::intmax_t m_height {};
    value_type m_max_value {};
};

} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PGM_IMAGE_H_H
