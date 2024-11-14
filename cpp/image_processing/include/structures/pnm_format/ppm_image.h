#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PPM_IMAGE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PPM_IMAGE_H_H

#include "pnm_image_base.h"
#include "structures/colors/rgb_color.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

namespace Marvin
{

namespace PNM_Format
{

class PPMImage : public PNMImageBase
{
  public:
    void read_raster_plain(std::istream& stream)
    {
        const auto pixel_count {width() * height()};
        if (static_cast<size_t>(pixel_count) >= m_pixels.max_size())
        {
            std::cerr << "Cannot allocate more than max ("
                      << m_pixels.max_size() << ") allowed elements.\n";
            return;
        }

        m_pixels.reserve(pixel_count);
        for (int i {0}; i < pixel_count; ++i)
        {
            RGBColor::value_type red {}, green {}, blue {};
            stream >> red >> green >> blue;
            m_pixels.emplace_back(red, green, blue);
        }

        for (std::intmax_t row {0}; row < height(); ++row)
        {
            for (std::intmax_t col {0}; col < width(); ++col)
            {
                std::cout << m_pixels[row * width() + col] << ' ';
            }
            std::cout << '\n';
        }
    }

    void read_raster_raw(std::istream& stream)
    {
        const auto pixel_count {width() * height()};
        if (static_cast<size_t>(pixel_count) >= m_pixels.max_size())
        {
            std::cerr << "Cannot allocate more than max ("
                      << m_pixels.max_size() << ") allowed elements.\n";
            return;
        }

        m_pixels.reserve(pixel_count);
        for (int k {0}; k < pixel_count; ++k)
        {
            std::array<RGBColor::value_type, 3> values {};
            for (int i {0}; i < 3; ++i)
            {
                if (max_value() > 0xFF)
                {
                    const auto first {stream.get()};
                    const auto second {stream.get()};
                    values[i] = (first << 8) | second;
                }
                else
                {
                    values[i] = stream.get();
                }
            }
            m_pixels.emplace_back(values);
        }
    }

    void read_from(std::string_view file_path)
    {
        try
        {
            std::ifstream file_stream {file_path.data(), std::ios::binary};
            if (!file_stream.good())
            {
                std::cerr << "Failed to open file " << file_path
                          << " for reading.\n";
                return;
            }

            std::string format_id {};
            file_stream >> format_id;

            const bool is_plain {
                is_plain_id(PNMImageBase::get_header_type(format_id))};

            // If is plain format, then reopen the stream to read as text.
            file_stream.close();
            file_stream.open(file_path.data(),
                is_plain ? std::ios_base::binary : std::ios_base::openmode {});

            if (!file_stream.good())
            {
                std::cerr << "Failed to reopen file " << file_path
                          << " for reading.\n";
                return;
            }

            PNMImageBase::read_header(file_stream);

            if (is_plain)
            {
                read_raster_plain(file_stream);
            }
            else
            {
                read_raster_raw(file_stream);
            }
        }
        catch (const std::ios_base::failure& failure)
        {
            std::cerr << "PPMImage: failed to read from file " << file_path
                      << "; what(): " << failure.what() << "\n";
        }
    }

    void write_raster_plain(std::ostream& stream)
    {
        constexpr auto line_limit {70};

        const auto w {std::to_string(max_value()).length()};

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
            stream << std::setw(w) << color.red() << ' '
                   << std::setw(w) << color.green() << ' ' << std::setw(w)
                   << color.blue() << ' ';
        }
    }

    void write_raster_raw(std::ostream& stream)
    {
        for (const auto& color : m_pixels)
        {
            std::array<Marvin::RGBColor::value_type, 3> values {
                color.red(), color.green(), color.blue()};
            for (int i {0}; i < 3; ++i)
            {
                if (max_value() > 0xFF)
                {
                    stream << static_cast<char>((values[i] >> 8) & 0xFF);
                    stream << static_cast<char>(values[i] & 0xFF);
                }
                else
                {
                    stream << static_cast<char>(values[i]);
                }
            }
        }
    }

    void write_to(std::string_view file_path, PNMImageBase::Mode mode,
        std::string_view comment = "")
    {
        try
        {
            std::ofstream file_stream {
                file_path.data(), (mode == PNMImageBase::Mode::raw)
                                      ? std::ios::binary | std::ios::out
                                      : std::ios::out};

            if (!file_stream.good())
            {
                std::cerr << "Failed to open file " << file_path
                          << " for writing.\n";
                return;
            }

            PNMImageBase::write_header(file_stream,
                (mode == PNMImageBase::Mode::plain) ? PNMImageBase::Type::P3
                                                    : PNMImageBase::Type::P6,
                comment);

            if (mode == PNMImageBase::Mode::raw)
            {
                write_raster_raw(file_stream);
            }
            else
            {
                write_raster_plain(file_stream);
            }
        }
        catch (const std::ios_base::failure& failure)
        {
            std::cerr << "PPMImage: failed to write to file " << file_path
                      << "; what(): " << failure.what() << "\n";
        }
    }

  private:
    std::vector<RGBColor> m_pixels {};
};

} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PPM_IMAGE_H_H
