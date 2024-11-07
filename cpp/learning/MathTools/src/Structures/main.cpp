#include "Structures/CartesianCooridinates.h"
#include "Structures/PointCartesian2D.h"
#include "Structures/MatrixFixed.h"

#include <iostream>

static void test_cartesian()
{
    Marvin::CartesianCooridinates<int, 2> point {};
    for (int i = 0; i < point.length(); ++i)
    {
        point[i] = (i + 7) * (i + 25);
        std::cout << point[i] << " ";
    }
    std::cout << "\n";

    Marvin::PointCartesian2D<int> p {3, 8};
    std::cout << p.x() << ", " << p.y() << "\n";
    p.set_to(23, -82);
    std::cout << p << "\n";
}

static void test_matrix()
{
    constexpr int row_count {23};
    constexpr int col_count {9};
    Marvin::MatrixFixed<int, row_count, col_count> matrix {};
    for (int i {0}; i < matrix.row_count(); ++i)
    {
        for (int j {0}; j < matrix.column_count(); ++j)
        {
            matrix[i, j] = i * 2 + j * j;
        }
    }
    matrix[4,3]=-100;

    std::cout << matrix;
    auto matrix2 = Marvin::identity_matrix<int, 3>();
    std::cout << matrix2;
}

int main()
{
    test_cartesian();
    test_matrix();

    return 0;
}
