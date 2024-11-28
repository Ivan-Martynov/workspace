#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_PNM_image_BASE_H_H

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

/*
Write/read to file. Use format to identify how to write header.
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

 *
 */

#include "structures/matrix_container.h"
#include "structures/colors/rgb_color.h"
#include "structures/colors/grayscale_color.h"
#include "structures/colors/blackwhite_color.h"

#include <memory>
#include <fstream>

namespace Marvin
{

namespace PNM_Format
{

/**
 * @brief Concept representing types for PNM images.
 * 
 * @tparam T Pixel color type.
 */
template <class T>
concept pnm_suitable
    = std::is_same_v<T, RGBColor> || std::is_same_v<T, GrayScaleColor>
      || std::is_same_v<T, BlackWhiteColor>;

/**
 * @brief Interface for PNM images.
 */
class IPNMImage
{
  public:
    virtual ~IPNMImage() = default;

    /**
     * @brief Read from a file.
     *
     * @param[in] file_path Path to the file.
     * @param[in] open_mode Mode to open the file. Default is binary.
     */
    virtual void read_from(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary)
        = 0;

    /**
     * @brief Write to a file.
     *
     * @param[in] file_path Path to the file.
     * @param[in] comment Comment to add to the header. Default is empty.
     * @param[in] open_mode Mode to open the file. Default is binary.
     */
    virtual void write_to(const char* const file_path,
        std::string_view comment = std::string_view {},
        std::ios_base::openmode open_mode = std::ios_base::binary) const
        = 0;

    /**
     * @brief Write to a file.
     *
     * @param[in] file_path Path to the file.
     * @param[in] open_mode Mode to open the file.
     * @param[in] comment Comment to add to the header. Default is empty.
     */
    virtual void write_to(const char* const file_path,
        std::ios_base::openmode open_mode,
        std::string_view comment = std::string_view {}) const
        = 0;
};

template <pnm_suitable T>
class PNMImageTemplated : public MatrixTemplated<T>, public IPNMImage
{
  public:
    using value_type = MatrixTemplated<T>::value_type;
    using size_type = MatrixTemplated<value_type>::size_type;

    /**
     * @brief Default emtpy constructor.
     */
    PNMImageTemplated() = default;

    /***************************************************************************
     * Constructors/destructor section.                                        *
     **************************************************************************/

    /**
     * @brief Initialize an image from width, height and max value.
     *
     * @tparam T Color type.
     * @param[in] width Width of the image.
     * @param[in] height Height of the image.
     * @param[in] max_value Max value for each channel pixel value.
     */
    PNMImageTemplated(size_type width, size_type height, size_type max_value);

    /**
     * @brief Initialize an image from width and height, using default max
     * value.
     *
     * @tparam T Color type.
     * @param[in] width Width of the image.
     * @param[in] height Height of the image.
     */
    PNMImageTemplated(size_type width, size_type height);

    /**
     * @brief Construct an image from a file.
     *
     * @tparam T Color type.
     * @param[in] file_path Path to the file to read from.
     * @param[in] open_mode Mode to open the file. Default is binary.
     */
    PNMImageTemplated(std::string_view file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary);

    ~PNMImageTemplated() = default;

    /***************************************************************************
     * End of Constructors/destructor section.                                 *
     **************************************************************************/

    /***************************************************************************
     * Getters section.                                                        *
     **************************************************************************/

    /**
     * @brief Width of the image, that is the number of columns in the
     * underlying matrix.
     *
     * @return size_type Width (number of columns).
     */
    size_type width() const { return this->column_count(); }

    /**
     * @brief Height of the image, that is the number of rows in the
     * underlying matrix.
     *
     * @return size_type Height (number of rows).
     */
    size_type height() const { return this->row_count(); }

    /**
     * @brief Maximum value for each channel in each pixel of the raster data of
     * the image.
     *
     * @return size_type Max value.
     */
    size_type max_value() const { return m_max_value; }

    /***************************************************************************
     * End of Getters section.                                                 *
     **************************************************************************/

    /***************************************************************************
     * Modifiers section.                                                      *
     **************************************************************************/

    /**
     * @brief Resize an image.
     *
     * @tparam T Color type.
     * @param[in] new_width New width.
     * @param[in] new_height New height.
     */
    void resize(size_type new_width, size_type new_height);

    /**
     * @brief Scale (resize) an image.
     *
     * @tparam T Color type.
     * @param[in] scale_value Value to scale the image with.
     */
    void scale(double scale_value);

    /***************************************************************************
     * End of Modifiers section.                                               *
     **************************************************************************/

    /***************************************************************************
     * Read/Write section.                                                     *
     **************************************************************************/

    /**
     * @brief Read from a file.
     *
     * @param[in] file_path Path to the file.
     * @param[in] open_mode Mode to open the file. Default is binary.
     */
    void read_from(const char* const file_path,
        std::ios_base::openmode open_mode = std::ios_base::binary) override;

    /**
     * @brief Write to a file.
     *
     * @param[in] file_path Path to the file.
     * @param[in] comment Comment to add to the header. Default is empty.
     * @param[in] open_mode Mode to open the file. Default is binary.
     */
    void write_to(const char* const file_path,
        std::string_view comment = std::string_view {},
        std::ios_base::openmode open_mode
        = std::ios_base::binary) const override;

    /**
     * @brief Write to a file.
     *
     * @param[in] file_path Path to the file.
     * @param[in] open_mode Mode to open the file.
     * @param[in] comment Comment to add to the header. Default is empty.
     */
    void write_to(const char* const file_path,
        std::ios_base::openmode open_mode,
        std::string_view comment = std::string_view {}) const override;

    /***************************************************************************
     * End of Read/Write section.                                              *
     **************************************************************************/
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
