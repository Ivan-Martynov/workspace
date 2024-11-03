#ifndef H_INCLUDE_STRUCTURES_CARTESIANCOORIDINATES_H_H
#define H_INCLUDE_STRUCTURES_CARTESIANCOORIDINATES_H_H

namespace Marvin
{

template <typename T, int N>
class CartesianCooridinates
{
  public:
    using value_type = T;
    using size_type = int;

  private:
    value_type m_coordinates[N] {};
    int m_length {N};

  public:
    explicit constexpr CartesianCooridinates() {}

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
};

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_CARTESIANCOORIDINATES_H_H
