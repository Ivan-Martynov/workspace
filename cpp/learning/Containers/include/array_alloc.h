#ifndef H_INCLUDE_ARRAY_ALLOC_H_H
#define H_INCLUDE_ARRAY_ALLOC_H_H

#include <iostream>

namespace Marvin
{

/**
 * @brief A dynamically allocated array of elements.
 *
 * @remarks The class is only for educational purposes. In practice, one should
 * prefer the std::vector<T> to use in their code.
 *
 * @tparam T Type of elements.
 */
template <class T>
class array_alloc
{
  public:
    using value_type = T;
    using size_type = int;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    /***************************************************************************
     * Constructors & destructor                                               *
     **************************************************************************/

    /**
     * @brief Default constructor: construct an empty array.
     */
    explicit array_alloc() : m_data_ptr {nullptr} {}

    /**
     * @brief Construct an array of given size and fill with the type's default
     * value.
     *
     * @param[in] n Length of array to construct.
     */
    explicit array_alloc(size_type n)
        : m_data_ptr {m_try_allocate(
              n, "array_alloc constructor: Failed to allocate memory")},
          m_length {n}, m_capacity {n}
    {
    }

    /**
     * @brief Construct an array of given size and fill with the specified
     * value.
     *
     * @param[in] n Length of array to construct.
     * @param[in] value Value to fill with.
     */
    explicit array_alloc(size_type n, const value_type& value) : array_alloc(n)
    {
        for (size_type i {0}; i < m_length; ++i)
        {
            m_data_ptr[i] = value;
        }
    }

    /**
     * @brief Copy constructor. Allocate new block of memory and copy elements.
     *
     * @param[in] other Array to copy from.
     */
    explicit array_alloc(const array_alloc& other)
        : m_data_ptr {m_try_allocate(other.length(),
              "array_alloc copy constructor: Failed to allocate memory")},
          m_length {other.length()}, m_capacity {other.capacity()}
    {
        m_copy_from_other(other);
    }

    /**
     * @brief Copy assignment. Allocate new block of memory and copy elements.
     *
     * @param[in] other Array to copy from.
     * @return array_alloc& Reference to the current object.
     */
    array_alloc& operator=(const array_alloc& other)
    {
        if (this != &other)
        {
            delete[] m_data_ptr;
            m_data_ptr = m_try_allocate(other.capacity(),
                "array_alloc copy assignment: Failed to allocate memory");

            m_copy_from_other(other);

            m_length = other.length();
            m_capacity = other.capacity();
        }
        return *this;
    }

    /**
     * @brief Move constructor. Move pointer's ownership.
     *
     * @param[in] other Array to move.
     */
    array_alloc(array_alloc&& other) noexcept
        : m_data_ptr {other.data()}, m_length {other.length()},
          m_capacity {other.capacity()}
    {
        other.m_reset();
    }

    /**
     * @brief Move assignment. Move pointer's ownership.
     *
     * @param[in] other Array to move.
     */
    array_alloc& operator=(array_alloc&& other) noexcept
    {
        if (this != &other)
        {
            delete[] m_data_ptr;

            m_data_ptr = other.data();
            m_length = other.length();
            m_capacity = other.capacity();

            other.m_reset();
        }
        return *this;
    }

    /**
     * @brief Construct an array from initializer list.
     * @remark The std::initializer_list is a view, therefore passed by value.
     *
     * @param[in] list List to copy elements from.
     */
    explicit array_alloc(std::initializer_list<value_type> list)
        : array_alloc(static_cast<size_type>(list.size()))
    {
        const auto n {static_cast<size_type>(list.size())};
        for (size_type i {0}; i < n; ++i)
        {
            m_data_ptr[i] = *(list.begin() + i);
        }
    }

    /**
     * @brief Assign from initializer list.
     *
     * @param[in] list List to copy elements from.
     */
    array_alloc& operator=(std::initializer_list<value_type> list)
    {
        const auto n {static_cast<size_type>(list.size())};
        if (n != m_length)
        {
            delete[] m_data_ptr;
            m_data_ptr
                = m_try_allocate(n, "array_alloc copy assignment from list "
                                    "initializer: Failed to allocate memory");

            m_capacity = m_length = n;
        }

        for (size_type i {0}; i < n; ++i)
        {
            m_data_ptr[i] = *(list.begin() + i);
        }

        return *this;
    }

    /**
     * @brief Destroy the array alloc object
     */
    ~array_alloc()
    {
        delete[] m_data_ptr;
        m_data_ptr = nullptr;
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
    size_type length() const
    {
        return m_length;
    }

    /**
     * @brief Capacity of the array.
     *
     * @return size_type Number of elements the array can hold without resizing.
     */
    size_type capacity() const
    {
        return m_capacity;
    }

    /**
     * @brief Size of array in bytes.
     *
     * @return std::size_t Number of bytes.
     */
    std::size_t size_in_bytes() const
    {
        return m_length * sizeof(value_type);
    }

    /**
     * @brief Pointer to the underlying data, modifiable.
     *
     * @return value_type* Pointer to the data.
     */
    value_type* data()
    {
        return m_data_ptr;
    }

    /**
     * @brief Pointer to the underlying data, non-modifiable.
     *
     * @return value_type* Pointer to the data.
     */
    const value_type* data() const
    {
        return m_data_ptr;
    }

    /**
     * @brief Index operator, modifiable.
     *
     * @param[in] i Index.
     * @return value_type& reference to the current element in the underlying
     * array.
     */
    value_type& operator[](size_type i)
    {
        return m_data_ptr[i];
    }

    /**
     * @brief Index operator, non-modifiable.
     *
     * @param[in] i Index.
     * @return value_type& reference to the current element in the underlying
     * array.
     */
    const value_type& operator[](size_type i) const
    {
        return m_data_ptr[i];
    }

    /***************************************************************************
     * End of Accessors section                                                *
     **************************************************************************/

    /***************************************************************************
     * Resize section                                                          *
     **************************************************************************/

    /**
     * @brief Reserve a block of memory for the array.
     *
     * @param[in] n Number of elements to reserve for.
     */
    void reserve(size_type n)
    {
        if (n > capacity())
        {
            value_type* temp_ptr {m_try_allocate(
                n, "array_alloc reserve function: Failed to allocate memory")};

            for (size_type i {0}; i < m_length; ++i)
            {
                temp_ptr[i] = m_data_ptr[i];
            }
            delete[] m_data_ptr;
            m_data_ptr = temp_ptr;
            m_capacity = n;
        }
    }

    /**
     * @brief Resize an array.
     *
     * @param[in] n New size.
     * @param[in] value Value to initialize elements with if any (default is the
     * default value of value_type).
     */
    void resize(size_type n, const value_type& value = value_type {})
    {
        if (n > m_length)
        {
            value_type* temp_ptr {m_try_allocate(
                n, "array_alloc resize function: Failed to allocate memory")};

            for (size_type i {0}; i < m_length; ++i)
            {
                temp_ptr[i] = m_data_ptr[i];
            }
            delete[] m_data_ptr;
            m_data_ptr = temp_ptr;

            for (size_type i {m_length}; i < n; ++i)
            {
                m_data_ptr[i] = value;
            }

            m_length = n;
            if (m_length > m_capacity)
            {
                m_capacity = m_length;
            }
        }
        else if (n < m_length)
        {
            m_length = n;
        }
    }

    /***************************************************************************
     * End of Resize section                                                   *
     **************************************************************************/

    /***************************************************************************
     * Element insertion section                                               *
     **************************************************************************/

    /**
     * @brief Append a new element to the end of array.
     *
     * @param[in] item Element to add.
     * @return array_alloc& Reference to the current object.
     */
    array_alloc& push_back(const_reference item)
    {
        if (m_length == m_capacity)
        {
            reserve(m_calculate_new_capacity());
        }

        m_data_ptr[m_length++] = item;

        return *this;
    }

    /**
     * @brief Create a new element at the end of array.
     *
     * @tparam Args Types of argument for element creation.
     * @param[in] args Parameters for the new element.
     * @return array_alloc& Reference to the current object.
     */
    template <typename... Args>
    array_alloc& emplace_back(Args&&... args)
    {
        if (m_length == m_capacity)
        {
            reserve(m_calculate_new_capacity());
        }

        m_data_ptr[m_length++] = value_type {std::forward<Args>(args)...};

        return *this;
    }

    /***************************************************************************
     * End of Element insertion section                                        *
     **************************************************************************/

  private:
    // Pointer to the underlying raw array.
    value_type* m_data_ptr {nullptr};
    // Length (size) of the array.
    size_type m_length {0};
    // Capacity of the array: how many elements the array can hold without
    // resizing.
    size_type m_capacity {0};

    /**
     * @brief Try allocating n elements for the array.
     *
     * @param[in] element_count Number of elements to allocate for.
     * @return value_type* Pointer to the allocated array.
     */
    value_type* m_try_allocate(int element_count, std::string_view message)
    {
        try
        {
            return new value_type[element_count] {};
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << message << ": (" << e.what() << ")\n";
            // Throw the same exception again. Note, no need to provide the
            // exception specifically, otherwise it may perform copy and
            // potential slicing may occur.
            throw;
        }
    }

    /**
     * @brief Try copying from another array. Inform the user if the data
     * pointer is null (that is, previous allocation has failed).
     *
     * @param[in] other Array to copy from.
     */
    void m_copy_from_other(const array_alloc& other)
    {
        for (size_type i {0}; i < other.length(); ++i)
        {
            m_data_ptr[i] = other[i];
        }
    }

    /**
     * @brief Reset array to a valid (in this case, 'zeroed') state. Used in
     * move operations.
     */
    void m_reset() noexcept
    {
        m_data_ptr = nullptr;
        m_length = 0;
        m_capacity = 0;
    }

    /**
     * @brief Calculate new capacity in case of array increase (e.g., after
     * inserting a new element).
     *
     * @return size_type Value to use for new capacity.
     */
    size_type m_calculate_new_capacity() const
    {
        // Arbitrarily chosen values: 4 for the minimum initial capacity and 1.6
        // as a factor and adding 0.5f to imitate rounding rules.
        return m_capacity > 4 ? static_cast<size_type>(m_capacity * 1.6f + 0.5f)
                              : 4;
    }
};

} // namespace Marvin

#endif // H_INCLUDE_ARRAY_ALLOC_H_H
