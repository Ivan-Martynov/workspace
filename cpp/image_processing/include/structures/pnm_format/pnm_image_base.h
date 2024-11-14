#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H

#include <array>
#include <string_view>
#include <cstdint>
#include <utility>

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

    
} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
