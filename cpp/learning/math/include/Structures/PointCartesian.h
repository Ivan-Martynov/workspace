#ifndef H_INCLUDE_STRUCTURES_POINTCARTESIAN_H_H
#define H_INCLUDE_STRUCTURES_POINTCARTESIAN_H_H

#include "CartesianCooridinates.h"

namespace Marvin
{

template <typename T, int N>
class PointCartesian : public CartesianCooridinates<T, N>
{
  public:
    using value_type = CartesianCooridinates<T, N>::value_type;
    using size_type = CartesianCooridinates<value_type, N>::size_type;

    explicit PointCartesian(std::span<const value_type>& array)
        : CartesianCooridinates<value_type, N> {array}
    {
    }

    explicit PointCartesian(std::initializer_list<value_type> list)
        : CartesianCooridinates<value_type, N> {list}
    {
    }

  private:
};


} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_POINTCARTESIAN_H_H
