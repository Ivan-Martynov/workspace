#ifndef H_INCLUDE_ARRAY_FIXED_H_H
#define H_INCLUDE_ARRAY_FIXED_H_H

#include <initializer_list>

namespace Marvin
{

template <typename T, int N>
class ArrayFixed
{
  public:
    using value_type = T;
    using size_type = int;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    /***************************************************************************
     * Constructors & destructor section                                       *
     **************************************************************************/

    /**
     * @brief Default constructor: construct an empty array.
     */
    explicit constexpr ArrayFixed() : m_data {} {}

    /**
     * @brief Construct an array filled with given value.
     *
     * @param[in] value Value to assign to every element.
     */
    explicit constexpr ArrayFixed(const value_type& value) : ArrayFixed {}
    {
        fill(value);
    }

    /**
     * @brief Construct an array from initializer list.
     * @remark The std::initializer_list is a view, therefore passed by value.
     *
     * @param[in] list List to copy elements from.
     */
    explicit constexpr ArrayFixed(std::initializer_list<value_type> list)
        : m_data {}
    {
        size_type i {0};
        auto it {list.begin()};
        while ((i < N) && (it != list.end()))
        {
            m_data[i++] = *it++;
        }
    }

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
    size_type length() const { return N; }

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
    value_type* data() { return m_data; }

    /**
     * @brief Pointer to the underlying data, non-modifiable.
     *
     * @return value_type* Pointer to the data.
     */
    const value_type* data() const { return m_data; }

    /**
     * @brief Index operator, modifiable.
     *
     * @param[in] i Index.
     * @return value_type& reference to the current element in the underlying
     * array.
     */
    value_type& operator[](size_type i) { return m_data[i]; }

    /**
     * @brief Index operator, non-modifiable.
     *
     * @param[in] i Index.
     * @return value_type& reference to the current element in the underlying
     * array.
     */
    const value_type& operator[](size_type i) const { return m_data[i]; }

    /***************************************************************************
     * End of Accessors section                                                *
     **************************************************************************/

    /***************************************************************************
     * Modifiers section                                                       *
     **************************************************************************/

    void constexpr fill(const value_type& value)
    {
        for (auto& v : m_data)
        {
            v = value;
        }
    }

    /***************************************************************************
     * End of Modifiers section                                                *
     **************************************************************************/

    /***************************************************************************
     * Iterator section                                                        *
     **************************************************************************/

    struct Iterator
    {
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(pointer ptr = nullptr) : m_ptr {ptr} {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        Iterator& operator++()
        {
            ++m_ptr;
            return *this;
        }

        Iterator operator++(int)
        {
            auto temp {*this};
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& rhs) const
        {
            return m_ptr == rhs.m_ptr;
        }

        bool operator!=(const Iterator& rhs) const
        {
            return !(*this == rhs);
        }

      private:
        pointer m_ptr;
    };
    static_assert(std::forward_iterator<Iterator>);

    struct ConstIterator
    {
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;

        ConstIterator(pointer ptr = nullptr) : m_ptr {ptr} {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        ConstIterator& operator++()
        {
            ++m_ptr;
            return *this;
        }

        ConstIterator operator++(int)
        {
            auto temp {*this};
            ++(*this);
            return temp;
        }

        bool operator==(const ConstIterator& rhs) const
        {
            return m_ptr == rhs.m_ptr;
        }

        bool operator!=(const ConstIterator& rhs) const
        {
            return !(*this == rhs);
        }

      private:
        pointer m_ptr;
    };
    static_assert(std::forward_iterator<Iterator>);

    Iterator begin() { return Iterator {m_data}; }
    Iterator end() { return Iterator {&m_data[N]}; }

    ConstIterator begin() const { return ConstIterator {m_data}; }
    ConstIterator end() const { return ConstIterator {&m_data[N]}; }

    ConstIterator cbegin() const { return begin(); }
    ConstIterator cend() const { return end(); }

    /***************************************************************************
     * End of Iterator section                                                 *
     **************************************************************************/


  private:
    value_type m_data[N];
};

} // namespace Marvin

#endif // H_INCLUDE_ARRAY_FIXED_H_H
