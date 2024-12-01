#ifndef H_INCLUDE_STRUCTURES_GEOMETRIC_POINT2D_H_H
#define H_INCLUDE_STRUCTURES_GEOMETRIC_POINT2D_H_H

#include <type_traits>

namespace Marvin
{
namespace Geometric
{

template <typename T>
    requires std::is_arithmetic_v<T>
class Point2dTemplated
{
  public:
    using value_type = T;

    /***************************************************************************
     * Constructors section.                                                   *
     **************************************************************************/

    explicit constexpr Point2dTemplated(value_type x, value_type y)
        : m_x {x}, m_y {y}
    {
    }
    explicit constexpr Point2dTemplated()
        : Point2dTemplated(value_type {}, value_type {})
    {
    }

    /***************************************************************************
     * End of Constructors section.                                            *
     **************************************************************************/

    /***************************************************************************
     * Getters section.                                                        *
     **************************************************************************/

    constexpr value_type x() const { return m_x; }
    constexpr value_type y() const { return m_y; }

    /***************************************************************************
     * End of Getters section.                                                 *
     **************************************************************************/

  private:
    value_type m_x {};
    value_type m_y {};
};

using Point2d = Point2dTemplated<double>;
using Point2i = Point2dTemplated<int>;

} // namespace Geometric


} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_GEOMETRIC_POINT2D_H_H
