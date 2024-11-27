#ifndef H_INCLUDE_STRUCTURES_MATRIX_CONTAINER_H_H
#define H_INCLUDE_STRUCTURES_MATRIX_CONTAINER_H_H

#include <vector>
#include <iostream>
#include <exception>

namespace Marvin
{

template <class T>
class MatrixTemplated
{
  public:
    using value_type = T;
    using size_type = int;

    /***************************************************************************
     * Construction section.                                                   *
     **************************************************************************/

    explicit MatrixTemplated() = default;
    explicit MatrixTemplated(size_type row_count, size_type column_count)
    try : m_values(row_count* column_count), m_row_count{row_count},
        m_column_count{column_count}
    {
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to create matrix of size (" << row_count << 'x'
                  << column_count << "): " << e.what() << "\n";
        throw;
    }
    catch (...)
    {
        std::cerr << "Failed to create matrix of size (" << row_count << 'x'
                  << column_count << "): unknown exception.\n";
        throw;
    }

    virtual ~MatrixTemplated() = default;

    explicit MatrixTemplated(const MatrixTemplated& other) = default;
    MatrixTemplated& operator=(const MatrixTemplated& other) = default;

    explicit MatrixTemplated(MatrixTemplated&& other)
        : m_values {std::move(other.m_values)}, m_row_count {other.m_row_count},
          m_column_count {other.m_column_count}
    {
    }

    MatrixTemplated& operator=(MatrixTemplated&& other)
    {
        m_values = std::move(other.m_values);
        m_row_count = other.m_row_count;
        m_column_count = other.m_column_count;

        return *this;
    }

    /***************************************************************************
     * End of Construction section.                                            *
     **************************************************************************/

    /***************************************************************************
     * Subscripting operations section.                                        *
     **************************************************************************/

    value_type& operator[](size_type i) { return m_values[i]; }
    const value_type& operator[](size_type i) const { return m_values[i]; }

    value_type& at(size_type i) { return m_values.at(i); }
    const value_type& at(size_type i) const { return m_values.at(i); }

    value_type& operator[](size_type i, size_type j)
    {
        return (*this)[m_index(i, j)];
    }
    const value_type& operator[](size_type i, size_type j) const
    {
        return (*this)[m_index(i, j)];
    }

    value_type& at(size_type i, size_type j) { return at(m_index(i, j)); }

    const value_type& at(size_type i, size_type j) const
    {
        return at(m_index(i, j));
    }

    /***************************************************************************
     * End of Subscripting operations section.                                 *
     **************************************************************************/

    /***************************************************************************
     * Data access section.                                                    *
     **************************************************************************/

    std::vector<value_type>& values() { return m_values; }
    const std::vector<value_type>& values() const { return m_values; }

    /***************************************************************************
     * End of Data access section.                                             *
     **************************************************************************/

    /***************************************************************************
     * Getters section.                                                        *
     **************************************************************************/

    size_type row_count() const { return m_row_count; }
    size_type column_count() const { return m_column_count; }

    /***************************************************************************
     * End of Getters section.                                                 *
     **************************************************************************/

  private:
    std::vector<value_type> m_values {};
    size_type m_row_count {};
    size_type m_column_count {};

    /**
     * @brief Caculate index for an underlying vector to retrive matrix element
     * specified by row and column.
     *
     * @param[in] i Row index.
     * @param[in] j Column index.
     *
     * @return constexpr size_type Index of the vector's item.
     */
    constexpr size_type m_index(size_type i, size_type j) const
    {
        return i * m_column_count + j;
    }
};

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_MATRIX_CONTAINER_H_H
