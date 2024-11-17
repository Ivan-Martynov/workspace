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

PNMImageBase::Type PNMImageBase::get_header_type(std::string_view format_id)
{
    for (size_t i {0}; i < PNMImageBase::type_names.size(); ++i)
    {
        if (PNMImageBase::type_names[i] == format_id)
        {
            return static_cast<PNMImageBase::Type>(i);
        }
    }

    return PNMImageBase::Type::max_type;
}

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

[[maybe_unused]]
static std::intmax_t s_get_int_value(std::istringstream& string_stream)
{
    std::intmax_t value {};
    string_stream >> value;
    return value;
}

[[maybe_unused]]
static void read_plain(std::istream& stream)
{
    std::string line {};
    std::getline(stream, line);

    if (line.length() > max_line_length)
    {
        std::cerr << "Line is too long: (max = " << max_line_length << ").\n";
    }

    std::istringstream string_stream {line};
    if (string_stream.peek() == '#')
    {
        string_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        std::intmax_t value {};
        string_stream >> value;
        if (value < 0)
        {
            std::cerr << "Wrong value.\n";
        }
    }
}

bool PNMImageBase::is_valid_type(Type type) const noexcept
{
    return type != PNMImageBase::Type::max_type;
}

bool PNMImageBase::is_valid_format_id(std::string_view format_id) const
{
    return get_header_type(format_id) != PNMImageBase::Type::max_type;
}

bool PNMImageBase::is_plain_id(Type type) const
{
    return (type == Type::P1) || (type == Type::P2) || (type == Type::P3);
}

void PNMImageBase::read_header(std::istream& stream)
{
    stream >> std::skipws;
    std::string format_id {};
    stream >> format_id;

    const auto type {get_header_type(format_id)};

    if (!is_valid_format_id(format_id))
    {
        std::cerr << "Reading header: invalid header identifier (got "
                  << format_id << ").\n";
        return;
    }

    skip_ws_and_comment(stream);

    stream >> m_width;
    std::cout << "Width = " << m_width << "\n";

    skip_ws_and_comment(stream);

    stream >> m_height;
    std::cout << "Height = " << m_height << "\n";

    skip_ws_and_comment(stream);

    if ((type != PNMImageBase::Type::P1) && (type != PNMImageBase::Type::P4))
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

void PNMImageBase::write_header(
    std::ostream& stream, Type type, std::string_view comment) const
{
    if (!is_valid_type(type))
    {
        std::cerr << "Invalid type.\n";
        return;
    }

    stream << type_names[std::to_underlying(type)] << '\n';
    if (!comment.empty())
    {
        stream << "# " << comment << '\n';
    }
    stream << m_width << ' ' << m_height << '\n';

    if ((type != PNMImageBase::Type::P1) && (type != PNMImageBase::Type::P4))
    {
        stream << m_max_value << '\n';
    }
}

std::intmax_t PNMImageBase::width() const noexcept
{
    return m_width;
}

std::intmax_t PNMImageBase::height() const noexcept
{
    return m_height;
}

std::intmax_t PNMImageBase::max_value() const noexcept
{
    return m_max_value;
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

        std::unique_ptr<IPNMImage> image_ptr {nullptr};
        if (PNMHeader::is_blackwhite(type))
        {
            image_ptr = std::make_unique<BWImage>();
        }
        else if (PNMHeader::is_grayscale(type))
        {
            image_ptr = std::make_unique<GSImage>();
        }
        else if (PNMHeader::is_rgb(type))
        {
            image_ptr = std::make_unique<RGBImage>();
        }
        else
        {
            std::cerr << "Wrong image header id.\n";
            return nullptr;
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
