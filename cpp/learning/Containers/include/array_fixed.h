#ifndef H_INCLUDE_ARRAY_FIXED_H_H
#define H_INCLUDE_ARRAY_FIXED_H_H

namespace Marvin
{

template <typename T, int N>
class array_fixed
{
  public:
    using value_type = T;
    using size_type = int;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

  private:
    value_type m_data[N];
};

} // namespace Marvin

#endif // H_INCLUDE_ARRAY_FIXED_H_H
