#ifndef H_INCLUDE_STRUCTURES_POINTCARTESIAN2D_H_H
#define H_INCLUDE_STRUCTURES_POINTCARTESIAN2D_H_H

#include "PointCartesian.h"

namespace Marvin
{

template <typename T>
class PointCartesian2D : public PointCartesian<T, 2>
{
  public:
    using value_type = PointCartesian<T, 2>::value_type;
    using size_type = PointCartesian<value_type, 2>::size_type;

    PointCartesian2D(value_type x, value_type y)
        : PointCartesian<value_type, 2>({x, y})
    {
    }

    const value_type& x() const
    {
        return (*this)[0];
    }

    const value_type& y() const
    {
        return (*this)[1];
    }

    void set_x(value_type x)
    {
        (*this)[0] = x;
    }

    void set_y(value_type y)
    {
        (*this)[1] = y;
    }

    void set_to(value_type x, value_type y)
    {
        set_x(x);
        set_y(y);
    }

  private:
};

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_POINTCARTESIAN2D_H_H
