#ifndef H_INCLUDE_STRUCTURES_CARTESIANCOORIDINATES_H_H
#define H_INCLUDE_STRUCTURES_CARTESIANCOORIDINATES_H_H

#include <span>
#include <ostream>

namespace Marvin
{

template <typename T, int N>
    requires std::floating_point<T> || std::integral<T>
class CartesianCooridinates
{
  public:
    using value_type = T;
    using size_type = int;

    explicit constexpr CartesianCooridinates() {}

    explicit CartesianCooridinates(std::span<const value_type>& array)
        : m_coordinates {}, m_length {static_cast<size_type>(array.size())}
    {
        for (size_t i {0}; i < array.size(); ++i)
        {
            m_coordinates[i] = array[i];
        }
    }

    explicit CartesianCooridinates(std::initializer_list<value_type> list)
        : m_coordinates {}, m_length {static_cast<size_type>(list.size())}
    {
        for (size_t i {0}; i < list.size(); ++i)
        {
            m_coordinates[i] = *(list.begin() + i);
        }
    }

    value_type& operator[](size_type i)
    {
        return m_coordinates[i];
    }

    const value_type& operator[](size_type i) const
    {
        return m_coordinates[i];
    }

    constexpr size_type length() const
    {
        return m_length;
    }

  private:
    value_type m_coordinates[N] {};
    int m_length {N};
};

template <typename T, int N>
std::ostream& operator<<(
    std::ostream& stream, const CartesianCooridinates<T, N>& coordinates)
{
    static_assert(N > 0);
    stream << "(" << coordinates[0];
    for (int i {1}; i < N; ++i)
    {
        stream << ", " << coordinates[i];
    }
    stream << ")";
    return stream;
}

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_CARTESIANCOORIDINATES_H_H
