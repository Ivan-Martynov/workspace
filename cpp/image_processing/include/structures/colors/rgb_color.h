#ifndef H_INCLUDE_STRUCTURES_COLORS_RGB_COLOR_H_H
#define H_INCLUDE_STRUCTURES_COLORS_RGB_COLOR_H_H

#include <cstdint>
#include <iostream>
#include <array>

namespace Marvin
{

class RGBColor
{
  public:
    using value_type = int;

    constexpr RGBColor(value_type red, value_type green, value_type blue)
        : m_red {red}, m_green {green}, m_blue {blue}
    {
    }

    constexpr RGBColor(const std::array<value_type, 3>& values)
        : RGBColor {values[0], values[1], values[2]}
    {
    }

    constexpr RGBColor() : RGBColor {0, 0, 0} {}

    static constexpr RGBColor from_intmax(std::intmax_t value)
    {
        return RGBColor {static_cast<value_type>(value >> 16 & 0xFF),
            static_cast<value_type>(value >> 8 & 0xFF),
            static_cast<value_type>(value & 0xFF)};
    }

    constexpr value_type red() const noexcept
    {
        return m_red;
    }

    constexpr value_type green() const noexcept
    {
        return m_green;
    }

    constexpr value_type blue() const noexcept
    {
        return m_blue;
    }

    constexpr operator std::intmax_t() const
    {
        return m_blue | (m_green << 8) | (m_red << 16);
    }

    friend std::istream& operator>>(std::istream& stream, RGBColor& color);

  private:
    value_type m_red;
    value_type m_green;
    value_type m_blue;
};

std::ostream& operator<<(std::ostream& stream, const RGBColor& color)
{
    stream << "(" << color.red() << ", " << color.green() << ", "
           << color.blue() << ")";
    return stream;
}

std::istream& operator>>(std::istream& stream, RGBColor& color)
{
    stream >> color.m_red >> color.m_green >> color.m_blue;
    return stream;
}

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_COLORS_RGB_COLOR_H_H
