#ifndef H_INCLUDE_ARRAY_FIXED_H_H
#define H_INCLUDE_ARRAY_FIXED_H_H

#include "ContainerHelper.h"

#include <initializer_list>

namespace Marvin
{

template <typename T, int N>
class ArrayFixed
{
  public:
    using value_type = T;
    using size_type = int;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /***************************************************************************
     * Constructors & destructor section                                       *
     **************************************************************************/

    /**
     * @brief Default constructor: construct an empty array.
     */
    constexpr ArrayFixed() : m_data {} {}

    /**
     * @brief Construct an array filled with given value.
     *
     * @param[in] value Value to assign to every element.
     */
    constexpr ArrayFixed(init_container_with_size_t, const value_type& value)
        : ArrayFixed {}
    {
        fill(value);
    }

    ArrayFixed(std::input_iterator auto first, std::input_iterator auto last)
        : m_data {}
    {
        for (size_type i {0}; (i < N) && (first != last); ++i, ++first) {
            m_data[i] = *first;
        }
    }

    /**
     * @brief Construct an array from initializer list.
     * @remark The std::initializer_list is a view, therefore passed by value.
     *
     * @param[in] list List to copy elements from.
     */
    constexpr ArrayFixed(std::initializer_list<value_type> list)
        : ArrayFixed {list.begin(), list.end()}
    {
    }

    ArrayFixed(const ArrayFixed& rhs) : ArrayFixed {rhs.cbegin(), rhs.cend()} {}

    ArrayFixed& operator=(const ArrayFixed& rhs)
    {
        if (this != &rhs) {
            for (size_type i {0}; i < N; ++i) {
                (*this)[i] = rhs[i];
            }
        }
        return *this;
    }

    ArrayFixed(ArrayFixed&& rhs) : ArrayFixed {} { swap(rhs); }

    ~ArrayFixed() noexcept {};

    /***************************************************************************
     * End of Constructors & destructor section                                *
     **************************************************************************/

    /***************************************************************************
     * Accessors section                                                       *
     **************************************************************************/

    /**
     * @brief Length (size) of the array.
     *
     * @return size_type Number of elements in the array.
     */
    constexpr size_type length() const { return N; }
    constexpr size_type max_length() const { return N; }

    constexpr size_type size() const { return N; }
    constexpr size_type max_size() const { return N; }

    bool empty() const { return N == 0; }

    /**
     * @brief Size of array in bytes.
     *
     * @return std::size_t Number of bytes.
     */
    std::size_t size_in_bytes() const { return N * sizeof(value_type); }

    /**
     * @brief Pointer to the underlying data, modifiable.
     *
     * @return value_type* Pointer to the data.
     */
    pointer data() { return std::addressof(m_data[0]); }

    /**
     * @brief Pointer to the underlying data, non-modifiable.
     *
     * @return value_type* Pointer to the data.
     */
    const_pointer data() const { return std::addressof(m_data[0]); }

    /**
     * @brief Index operator, modifiable.
     *
     * @param[in] i Index.
     * @return value_type& reference to the current element in the underlying
     * array.
     */
    reference operator[](size_type i) { return m_data[i]; }

    /**
     * @brief Index operator, non-modifiable.
     *
     * @param[in] i Index.
     * @return value_type& reference to the current element in the underlying
     * array.
     */
    const_reference operator[](size_type i) const { return m_data[i]; }

    reference at(size_type i)
    {
        m_check_index(i);
        return m_data[i];
    }

    const_reference at(size_type i) const
    {
        m_check_index(i);
        return m_data[i];
    }

    reference front() { return m_data[0]; }
    const_reference front() const { return m_data[0]; }

    reference back() { return m_data[N - 1]; }
    const_reference back() const { return m_data[N - 1]; }

    /***************************************************************************
     * End of Accessors section                                                *
     **************************************************************************/

    /***************************************************************************
     * Modifiers section                                                       *
     **************************************************************************/

    void constexpr fill(const value_type& value) noexcept
    {
        for (auto& v : m_data) {
            v = value;
        }
    }

    void swap(ArrayFixed& rhs) noexcept
    {
        for (size_type i {0}; i < N; ++i) {
            using std::swap;
            swap((*this)[i], rhs[i]);
        }
    }

    /***************************************************************************
     * End of Modifiers section                                                *
     **************************************************************************/

    /***************************************************************************
     * Iterator section                                                        *
     **************************************************************************/

    iterator begin() noexcept { return iterator {data()}; }
    const_iterator begin() const noexcept { return const_iterator {data()}; }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return iterator {data() + N}; }
    const_iterator end() const noexcept { return const_iterator {data() + N}; }
    const_iterator cend() const noexcept { return end(); }

    reverse_iterator rbegin() noexcept { return reverse_iterator {end()}; }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator {end()};
    }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return reverse_iterator {begin()}; }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator {begin()};
    }
    const_reverse_iterator crend() const noexcept { return rend(); }

    /***************************************************************************
     * End of Iterator section                                                 *
     **************************************************************************/

  private:
    value_type m_data[N ? N : 1];

    void m_check_index(size_type i) const
    {
        if ((i < 0) || (i >= N)) {
            throw std::out_of_range {"ArrayFixed Out of Range error."};
        }
    }
};

/*******************************************************************************
 * Comparison section                                                          *
 ******************************************************************************/

template <typename T, int N>
bool operator==(const ArrayFixed<T, N>& lhs, const ArrayFixed<T, N>& rhs)
{
    for (auto first {lhs.cbegin()}, second {rhs.cbegin()}; first != lhs.cend();
         ++first, ++second) {
        if (*first != *second) {
            return false;
        }
    }
    return true;
}

template <typename T, int N>
bool operator!=(const ArrayFixed<T, N>& lhs, const ArrayFixed<T, N>& rhs)
{
    return !(lhs == rhs);
}

/*******************************************************************************
 * End of Comparison section                                                   *
 ******************************************************************************/

template <typename T, int N>
void swap(ArrayFixed<T, N>& lhs, ArrayFixed<T, N>& rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace Marvin

#endif // H_INCLUDE_ARRAY_FIXED_H_H
