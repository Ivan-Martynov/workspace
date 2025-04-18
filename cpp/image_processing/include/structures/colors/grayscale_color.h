#ifndef H_INCLUDE_STRUCTURES_COLORS_GRAYSCALE_COLOR_H_H
#define H_INCLUDE_STRUCTURES_COLORS_GRAYSCALE_COLOR_H_H

#include <iostream>

namespace Marvin
{

class GrayScaleColor
{
  public:
    using value_type = int;

    constexpr GrayScaleColor(value_type value = 0) noexcept : m_value {value} {}

    constexpr value_type value() const noexcept {return m_value;}

    friend std::istream& operator>>(
        std::istream& stream, GrayScaleColor& color);

  private:
    value_type m_value;
};

inline std::ostream& operator<<(std::ostream& stream, const GrayScaleColor& color)
{
    return stream << color.value();
}

inline std::istream& operator>>(std::istream& stream, GrayScaleColor& color)
{
    return stream >> color.m_value;
}

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_COLORS_GRAYSCALE_COLOR_H_H
