#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PBM_IMAGE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PBM_IMAGE_H_H

#include "pnm_image_base.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <bitset>

namespace Marvin
{

namespace PNM_Format
{

class PBMImage : public PNMImageBase
{
  public:
    explicit PBMImage() = default;

    ~PBMImage() = default;

    explicit PBMImage(const PBMImage& other) = delete;
    PBMImage& operator=(const PBMImage& other) = delete;

    explicit PBMImage(PBMImage&& other) = default;
    PBMImage& operator=(PBMImage&& other) = default;

    void read_raster_plain(std::istream& stream)
    {
        if (static_cast<size_t>(width() * height()) >= m_pixels.max_size())
        {
            std::cerr << "Cannot allocate more than max ("
                      << m_pixels.max_size() << ") allowed elements.\n";
            return;
        }

        m_pixels.resize(width() * height());

        for (std::intmax_t row {0}; row < height(); ++row)
        {
            for (std::intmax_t col {0}; col < width(); ++col)
            {
                stream >> m_pixels[row * width() + col];
            }
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
        if (static_cast<size_t>(width() * height()) >= m_pixels.max_size())
        {
            std::cerr << "Cannot allocate more than max ("
                      << m_pixels.max_size() << ") allowed elements.\n";
            return;
        }
        m_pixels.reserve(width() * height());

        const auto add_pixel_lambda {[&stream, this](int bit_count)
            {
                const auto byte_value {stream.get()};
                auto shift {8};
                while (bit_count--)
                {
                    // Extract most significant bits first.
                    m_pixels.push_back((byte_value & 1 << --shift) != 0);
                }
            }};

        const auto row_value_count {width() / 8};
        const int last_value_bit_count {static_cast<int>(width() % 8)};

        for (std::intmax_t row {0}; row < height(); ++row)
        {
            for (std::intmax_t i {0}; i < row_value_count; ++i)
            {
                add_pixel_lambda(8);
            }

            if (last_value_bit_count != 0)
            {
                add_pixel_lambda(last_value_bit_count);
            }
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

    void write_raster_plain(std::ostream& stream)
    {
        for (std::intmax_t row {0}; row < height(); ++row)
        {
            for (std::intmax_t col {0}; col < width(); ++col)
            {
                stream << m_pixels[row * width() + col] << ' ';
            }
            stream << '\n';
        }
    }

    void write_raster_raw(std::ostream& stream)
    {
        const auto add_pixel_lambda {
            [&stream, this](std::intmax_t row, std::intmax_t col, int bit_count)
            {
                unsigned char byte_value {};
                unsigned char shift {8};
                while (bit_count--)
                {
                    // The most signficant bit is place first.
                    byte_value |= m_pixels[row * width() + col++] << --shift;
                }

                stream << byte_value;

                return col;
            }};

        const auto row_value_count {width() / 8};
        const int last_value_bit_count {static_cast<int>(width() % 8)};

        for (std::intmax_t row {0}; row < height(); ++row)
        {
            std::intmax_t col {0};
            for (std::intmax_t i {0}; i < row_value_count; ++i)
            {
                col = add_pixel_lambda(row, col, 8);
            }

            if (last_value_bit_count)
            {
                col = add_pixel_lambda(row, col, last_value_bit_count);
            }
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
            std::cerr << "PBMImage: failed to read from file " << file_path
                      << "; what(): " << failure.what() << "\n";
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
                (mode == PNMImageBase::Mode::plain) ? PNMImageBase::Type::P1
                                                    : PNMImageBase::Type::P4,
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
            std::cerr << "PBMImage: failed to write to file " << file_path
                      << "; what(): " << failure.what() << "\n";
        }
    }

  private:
    std::vector<int> m_pixels {};
};

} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PBM_IMAGE_H_H
