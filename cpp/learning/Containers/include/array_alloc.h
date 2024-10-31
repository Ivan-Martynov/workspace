#ifndef H_INCLUDE_ARRAY_ALLOC_H_H
#define H_INCLUDE_ARRAY_ALLOC_H_H

#include <iostream>

namespace Marvin
{

template <class T> class array_alloc
{
  public:
    using value_type = T;
    using size_type = int;

  private:
    size_type m_length {0};
    size_type m_capacity {0};
    value_type* m_data_ptr;

    void m_failed_alloc_info()
    {
        std::cerr << "array_alloc: Failed to allocate memory.\n";
    }

    void m_set_to(value_type value)
    {
        for (size_type i {0}; i < m_length; ++i)
        {
            m_data_ptr[i] = value;
        }
    }

    void m_set_from(const value_type* ptr, size_type n)
    {
        for (size_type i {0}; i < n; ++i)
        {
            m_data_ptr[i] = ptr[i];
        }
    }

    // Using std::nothrow to avoid crashing, but checking for nullptr later.
    explicit array_alloc(const value_type* ptr, const size_type n)
        : m_data_ptr {new(std::nothrow) value_type[m_length]}
    {
        if (!m_data_ptr)
        {
            m_failed_alloc_info();
        }
        else
        {
            m_length = n;
            m_capacity = n;
            m_set_from(ptr, m_length);
        }
    }

  public:
    explicit array_alloc() : m_data_ptr {nullptr} {}

    explicit array_alloc(size_type n, const value_type& value = value_type())
        : m_data_ptr {new(std::nothrow) value_type[m_length]}
    {
        if (!m_data_ptr)
        {
            m_failed_alloc_info();
        }
        else
        {
            m_length = n;
            m_capacity = n;
            m_set_to(value);
        }
    }

    explicit array_alloc(const array_alloc& other)
        : array_alloc(other.data(), other.length())
    {
    }

    array_alloc& operator=(const array_alloc& other)
    {
        if (this != &other)
        {
            m_data_ptr = new (std::nothrow) value_type[other.length()];
            if (!m_data_ptr)
            {
                m_failed_alloc_info();
            }
            else
            {
                m_length = other.length();
                m_capacity = other.capacity();

                m_set_from(other.data(), m_length);
            }
        }
        return *this;
    }

    ~array_alloc()
    {
        delete[] m_data_ptr;
        m_data_ptr = nullptr;
    }

    size_type length() const
    {
        return m_length;
    }

    size_type capacity() const
    {
        return m_capacity;
    }

    value_type* data()
    {
        return m_data_ptr;
    }

    const value_type* data() const
    {
        return m_data_ptr;
    }

    value_type& operator[](size_type i)
    {
        return m_data_ptr[i];
    }

    const value_type& operator[](size_type i) const
    {
        return m_data_ptr[i];
    }

    void reserve(size_type n)
    {
        if (n > capacity())
        {
            value_type* temp_ptr {new (std::nothrow) value_type[n]};
            if (!temp_ptr)
            {
                m_failed_alloc_info();
                return;
            }

            for (size_type i {0}; i < m_length; ++i)
            {
                temp_ptr[i] = operator[](i);
            }
            delete[] m_data_ptr;
            m_data_ptr = temp_ptr;
            m_capacity = n;
        }
    }

    void resize(size_type n, const value_type& value = value_type{})
    {
        if (length() < n)
        {
            value_type* temp_ptr {new (std::nothrow) value_type[n]};
            if (!temp_ptr)
            {
                m_failed_alloc_info();
                return;
            }

            for (size_type i {0}; i < m_length; ++i)
            {
                temp_ptr[i] = operator[](i);
            }
            delete[] m_data_ptr;
            m_data_ptr = temp_ptr;

            for (size_type i {m_length}; i < n; ++i)
            {
                m_data_ptr[i] = value;
            }

            m_length = n;
            if (m_length > capacity())
            {
                m_capacity = m_length;
            }
        }
        else if (length() > n)
        {
            m_length = n;
        }
    }
};

} // namespace Marvin

#endif // H_INCLUDE_ARRAY_ALLOC_H_H
