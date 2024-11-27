#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H

#include "structures/matrix_container.h"
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

/**
 * @brief Structure containing comments for PNM image header.
 *
 * @remark All comments can be empty.
 */
struct HeaderComments
{
    using value_type = std::string;

    // The main comment refers to the comment right after the header id (aka
    // magic number).
    value_type main_comment {};

    // Width comment refers to the comment after the width field. Rarely used.
    value_type width_comment {};

    // Height comment refers to the comment after the height field. Rarely used.
    value_type height_comment {};

    // Max value comment refers to the comment after the max value field. Rarely
    // used.
    value_type max_value_comment {};

    HeaderComments() = default;
    HeaderComments(std::string_view comment) : main_comment {comment} {}
};

template <class T>
concept pnm_suitable
    = std::is_same_v<T, RGBColor> || std::is_same_v<T, GrayScaleColor>
      || std::is_same_v<T, BlackWhiteColor>;

class IPNMImage
{
  public:
    virtual ~IPNMImage() = default;

    virtual void read_from(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary)
        = 0;

    virtual void write_to(const char* const file_path,
        const HeaderComments& comments,
        std::ios_base::openmode open_mode = std::ios_base::binary) const
        = 0;

    virtual void write_to(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary,
        const HeaderComments& comments = {}) const
        = 0;

    virtual void write_to(const char* const file_path,
        std::ios_base::openmode open_mode, std::string_view comment) const
        = 0;

    virtual void write_to(const char* const file_path, std::string_view comment,
        std::ios_base::openmode open_mode) const
        = 0;
};

template <pnm_suitable T>
class PNMImageTemplated : public MatrixTemplated<T>, public IPNMImage
{
  public:
    using value_type = MatrixTemplated<T>::value_type;
    using size_type = MatrixTemplated<value_type>::size_type;

    PNMImageTemplated() = default;
    PNMImageTemplated(size_type width, size_type height, size_type max_value);
    PNMImageTemplated(size_type width, size_type height);

    ~PNMImageTemplated() = default;

    size_type width() const { return this->column_count(); }
    size_type height() const { return this->row_count(); }
    size_type max_value() const { return m_max_value; }

    void resize(size_type new_width, size_type new_height);
    void scale(double scale_value);

    void read_from(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary) override;

    void write_to(const char* const file_path, const HeaderComments& comments,
        std::ios_base::openmode open_mode
        = std::ios_base::binary) const override;

    void write_to(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary,
        const HeaderComments& comments = {}) const override;

    void write_to(const char* const file_path,
        std::ios_base::openmode open_mode,
        std::string_view comment) const override;

    void write_to(const char* const file_path, std::string_view comment,
        std::ios_base::openmode open_mode
        = std::ios_base::binary) const override;

  private:
    size_type m_max_value {};
};

// Using more descriptive names for specific image classes.
using PBMImage = PNMImageTemplated<BlackWhiteColor>;
using PGMImage = PNMImageTemplated<GrayScaleColor>;
using PPMImage = PNMImageTemplated<RGBColor>;

/**
 * @brief Create an image pointer from a file.
 *
 * @param[in] file_path Path to the file.
 * @return std::unique_ptr<IPNMImage> Pointer to the created image.
 */
std::unique_ptr<IPNMImage> make_image_ptr(const char* const file_path);

} // namespace PNM_Format

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
