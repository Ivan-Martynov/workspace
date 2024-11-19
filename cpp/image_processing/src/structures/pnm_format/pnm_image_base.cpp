#include "structures/pnm_format/pnm_image_base.h"

#include <string>
#include <iostream>
#include <sstream>
#include <limits>

namespace Marvin
{

namespace PNM_Format
{

static constexpr int max_line_length {70};
static constexpr int absolute_max_value {65535}; // 2^16 - 1

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

PNMHeader::PNMHeader(std::istream& stream)
{
    stream >> std::skipws;
    std::string format_id {};
    stream >> format_id;

    m_type = type_from_string(format_id).value_or(PNMHeader::Type::max_type);

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

void PPMImage::value_to_ostream_plain(
    std::ostream& stream, const value_type& color, size_type item_width) const
{
    stream << std::setw(item_width) << color.red() << ' '
           << std::setw(item_width) << color.green() << ' '
           << std::setw(item_width) << color.blue();
}

void PPMImage::m_read_raster_data_raw(std::istream& stream)
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

void PPMImage::m_write_raster_data_raw(std::ostream& stream) const
{
    for (const auto& color : data())
    {
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

void PGMImage::m_read_raster_data_raw(std::istream& stream)
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

void PGMImage::m_write_raster_data_raw(std::ostream& stream) const
{
    if (max_value() > 0xFF)
    {
        for (const auto& color : data())
        {
            stream << static_cast<unsigned char>((color.value() >> 8) & 0xFF);
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

void PBMImage::m_read_raster_data_raw(std::istream& stream)
{
    int pixel_index {0};
    const auto add_pixel_lambda {[&stream, &pixel_index, this](int bit_count)
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

void PBMImage::m_write_raster_data_raw(std::ostream& stream) const
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

std::unique_ptr<IPNMImage> read_from_file(const char* const file_path)
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

        const auto type {PNMHeader::type_from_string(format_id)};
        if (!type)
        {
            std::cerr << "Wrong image header id.\n";
            return nullptr;
        }

        std::unique_ptr<IPNMImage> image_ptr {nullptr};
        if (PNMHeader::is_blackwhite(type.value()))
        {
            image_ptr = std::make_unique<PBMImage>();
        }
        else if (PNMHeader::is_grayscale(type.value()))
        {
            image_ptr = std::make_unique<PGMImage>();
        }
        else if (PNMHeader::is_rgb(type.value()))
        {
            image_ptr = std::make_unique<PPMImage>();
        }

        image_ptr->read_from(file_path);

#if 0
        file_stream.close();
        file_stream.open(file_path.data(),
            is_plain ? std::ios_base::openmode {} : std::ios_base::binary);

        if (!file_stream.good())
        {
            std::cerr << "Failed to reopen file " << file_path
                      << " for reading.\n";
            return nullptr;
        }

        const PNMHeader header {file_stream};
        if (!image_ptr->m_try_init_from_header(header))
        {
            return nullptr;
        }

        if (is_plain)
        {
            m_read_raster_data_plain(file_stream);
        }
        else
        {
            m_read_raster_data_raw(file_stream);
        }
#endif
        return image_ptr;
    }
    catch (const std::ios_base::failure& failure)
    {
        std::cerr << "PPMImage: failed to read from file " << file_path
                  << "; what(): " << failure.what() << "\n";
        return nullptr;
    }
}

} // namespace PNM_Format

} // namespace Marvin
