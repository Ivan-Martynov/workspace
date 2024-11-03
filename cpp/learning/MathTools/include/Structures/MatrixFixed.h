#ifndef H_INCLUDE_STRUCTURES_MATRIXFIXED_H_H
#define H_INCLUDE_STRUCTURES_MATRIXFIXED_H_H

#include "Algorithms/Numerical.h"

#include <iostream>
#include <utility> // For std::as_const.
#include <iomanip>

namespace Marvin
{

template <typename T, int N, int M> class MatrixFixed
{
    static_assert(N > 0 && M > 0, "Matrix cannot have zero rows or columns.");

  public:
    using value_type = T;
    using size_type = int;

  private:
    value_type m_cells[N * M] {};
    size_type m_row_count {N};
    size_type m_column_count {M};
    size_type m_count {N * M};

  public:
    explicit constexpr MatrixFixed() {}

#if 1
    const value_type& operator[](size_type i) const
    {
        return m_cells[i];
    }

    value_type& operator[](size_type i)
    {
        return const_cast<value_type&>(std::as_const(*this)[i]);
    }

    const value_type& operator[](size_type i, size_type j) const
    {
        return operator[](i * m_column_count + j);
    }

    value_type& operator[](size_type i, size_type j)
    {
        return const_cast<value_type&>(std::as_const(*this)[i, j]);
    }
#else
    constexpr auto&& operator[](
        this MatrixFixed&& self, size_type i, size_type j)
    {
        return std::forward<MatrixFixed>(self).m_cells[i * m_column_count + j];
    }
#endif

    constexpr size_type row_count() const
    {
        return m_row_count;
    }

    constexpr size_type column_count() const
    {
        return m_column_count;
    }
};

template <typename T, int N> MatrixFixed<T, N, N> identity_matrix()
{
    MatrixFixed<T, N, N> matrix {};
    for (int i {N - 1}; i >= 0; --i)
    {
        matrix[i, i] = 1;
    }
    return matrix;
}

template <typename T, int N, int M>
std::ostream& operator<<(
    std::ostream& stream, const MatrixFixed<T, N, M>& matrix)
{
    int columns_widths[M] {};

    for (int column_index {0}; column_index < matrix.column_count();
         ++column_index)
    {
        for (int row_index {0}; row_index < matrix.row_count(); ++row_index)
        {
            const auto width {
                number_digit_count<int>(matrix[row_index, column_index])};
            if (width > columns_widths[column_index])
            {
                columns_widths[column_index] = width;
            }
        }
    }

    auto print_value_lambda {[&stream](const T value, int w)
        { stream << std::setw(w) << std::right << value; }};

    stream << "Matrix[" << matrix.row_count() << "x" << matrix.column_count()
           << "]\n[";
    print_value_lambda(matrix[0, 0], columns_widths[0]);

    for (int j {1}; j < matrix.column_count(); ++j)
    {
        stream << " | ";
        print_value_lambda(matrix[0, j], columns_widths[j]);
    }

    for (int i {1}; i < matrix.row_count(); ++i)
    {
        stream << "\n" << ' ';
        print_value_lambda(matrix[i, 0], columns_widths[0]);
        for (int j {1}; j < matrix.column_count(); ++j)
        {
            stream << " | ";
            print_value_lambda(matrix[i, j], columns_widths[j]);
        }
    }
    stream << "]\n";

    return stream;
}

} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_MATRIXFIXED_H_H
