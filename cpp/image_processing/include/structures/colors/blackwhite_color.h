#ifndef H_INCLUDE_STRUCTURES_COLORS_BLACKWHITE_COLOR_H_H
#define H_INCLUDE_STRUCTURES_COLORS_BLACKWHITE_COLOR_H_H

#include <iostream>

namespace Marvin
{

class BlackWhiteColor
{
  public:
    using value_type = bool;

    constexpr BlackWhiteColor(value_type value = m_black) noexcept
        : m_value {value}
    {
    }

    constexpr value_type value() const noexcept
    {
        return m_value;
    }

    friend std::istream& operator>>(
        std::istream& stream, BlackWhiteColor& color);

  private:
    value_type m_value;

    static constexpr value_type m_black {false};
    static constexpr value_type m_white {true};
};

inline std::ostream& operator<<(std::ostream& stream, const BlackWhiteColor& color)
{
    return stream << color.value();
}

inline std::istream& operator>>(std::istream& stream, BlackWhiteColor& color)
{
    return stream >> color.m_value;
}

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_COLORS_BLACKWHITE_COLOR_H_H
