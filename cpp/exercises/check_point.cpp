#include <iostream>
#include <iomanip>
#include <string>

class Point3d
{
  private:
    double m_x;
    double m_y;
    double m_z;

  public:
    constexpr explicit Point3d(const double x, const double y, const double z)
        : m_x {x}, m_y {y}, m_z {z}
    {
    }

    constexpr double X() const
    {
        return m_x;
    }

    constexpr double Y() const
    {
        return m_y;
    }

    constexpr double Z() const
    {
        return m_z;
    }

    double dot_product_xy(const Point3d& other) const
    {
        return m_x * other.X() + m_y * other.Y();
    }

    double dot_product(const Point3d& other) const
    {
        return m_x * other.X() + m_y * other.Y() + m_z * other.Z();
    }

    static Point3d cross_product(const Point3d& lhs, const Point3d& rhs)
    {
        return Point3d {lhs.Y() * rhs.Z() - lhs.Z() * rhs.Y(),
            lhs.Z() * rhs.X() - lhs.X() * rhs.Z(),
            lhs.X() * rhs.Y() - lhs.Y() * rhs.X()};
    }

    static Point3d diff(const Point3d& lhs, const Point3d& rhs)
    {
        return Point3d {
            rhs.X() - lhs.X(), rhs.Y() - lhs.Y(), rhs.Z() - lhs.Z()};
    }

    static double skew_product_xy(const Point3d& lhs, const Point3d& rhs)
    {
        return lhs.X() * rhs.Y() - lhs.Y() * rhs.X();
    }

    std::string to_str_xy() const
    {
        return "(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ")";
    }

    std::string to_str() const
    {
        return "(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ", "
               + std::to_string(m_z) + ")";
    }
};

bool are_collinear_xy(
    const Point3d& first, const Point3d& second, const Point3d& test_point)
{
    const Point3d vec_1 {Point3d::diff(second, first)};
    const Point3d vec_2 {Point3d::diff(first, test_point)};
    const double skew {Point3d::skew_product_xy(vec_1, vec_2)};

    constexpr double tol = 0.0001;
    const bool collinear = abs(skew) < tol;

    std::cout << std::fixed;
    std::cout << "Points " << first.to_str_xy() << ", " << second.to_str_xy()
              << " and " << test_point.to_str_xy() << " are";

    if (!collinear)
    {
        std::cout << " not";
    }

    std::cout << " collinear in 2d (xy).\n";

    return collinear;
}

bool are_collinear(
    const Point3d& first, const Point3d& second, const Point3d& test_point)
{
    const Point3d vec_1 {Point3d::diff(second, first)};
    const Point3d vec_2 {Point3d::diff(first, test_point)};
    const Point3d cross {Point3d::cross_product(vec_1, vec_2)};

    constexpr double tol = 0.0001;
    const bool collinear
        = (cross.X() < tol) && (cross.Y() < tol) && (cross.Z() < tol);

    std::cout << std::fixed;
    std::cout << "Points " << first.to_str() << ", " << second.to_str()
              << " and " << test_point.to_str() << " are";

    if (!collinear)
    {
        std::cout << " not";
    }

    std::cout << " collinear.\n";

    return collinear;
}

bool is_point_between_xy(
    const Point3d& first, const Point3d& second, const Point3d& test_point)
{
    bool collinear = are_collinear_xy(first, second, test_point);

    if (collinear)
    {
        const Point3d vec_1 {Point3d::diff(first, test_point)};
        const Point3d vec_2 {Point3d::diff(second, test_point)};
        collinear = vec_1.dot_product_xy(vec_2) <= 0;
    }

    std::cout << std::fixed;
    std::cout << "Point " << test_point.to_str_xy() << " is ";
    if (!collinear)
    {
        std::cout << "not ";
    }
    std::cout << "between points " << first.to_str_xy() << " and "
              << second.to_str_xy() << " in 2d (xy).\n";

    return collinear;
}

bool is_point_between(
    const Point3d& first, const Point3d& second, const Point3d& test_point)
{
    bool collinear = are_collinear(first, second, test_point);

    if (collinear)
    {
        const Point3d vec_1 {Point3d::diff(first, test_point)};
        const Point3d vec_2 {Point3d::diff(second, test_point)};
        collinear = vec_1.dot_product(vec_2) <= 0;
    }

    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    std::cout << "Point " << test_point.to_str() << " is ";
    if (!collinear)
    {
        std::cout << "not ";
    }
    std::cout << "between points " << first.to_str() << " and "
              << second.to_str() << ".\n";

    return collinear;
}

int main()
{
    Point3d first_point {1.0, 1.0, 1.0};
    Point3d second_point {3.0, 3.0, 3.0};
    Point3d test_point {2.0, 2.0, 2.0};
    Point3d test_point2 {2.0, 4.0, 2.0};

    std::cout << first_point.dot_product(second_point) << '\n';
    std::cout << first_point.dot_product(test_point) << '\n';

#if 0
    are_collinear(first_point, second_point, test_point);
    are_collinear(first_point, second_point, test_point2);

    are_collinear_xy(first_point, second_point, test_point);
    are_collinear_xy(first_point, second_point, test_point2);
#endif

    is_point_between(first_point, second_point, test_point);
    is_point_between(first_point, test_point, second_point);
    is_point_between(first_point, second_point, test_point2);
    is_point_between(first_point, second_point, second_point);

    is_point_between_xy(first_point, second_point, test_point);
    is_point_between_xy(first_point, test_point, second_point);
    is_point_between_xy(first_point, second_point, test_point2);
    is_point_between_xy(first_point, second_point, first_point);

    return 0;
}
