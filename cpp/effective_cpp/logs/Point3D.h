#ifndef POINT3D_H
#define POINT3D_H

#include <fstream>
#include <algorithm>
#include "Utils.h"

namespace log_measurement {

// class for points in plane, represented by x and y coordinates
template <class T>
class Point3D {
    static_assert(std::is_arithmetic<T>::value, "Type should be arithmetic");
public:

    using value_type = T;

    constexpr Point3D() noexcept;
    explicit constexpr Point3D(const value_type, const value_type,
            const value_type) noexcept;

    constexpr Point3D(const Point3D&) = default;
    constexpr Point3D& operator=(const Point3D&) = default;
    constexpr Point3D(Point3D&&) = default;
    constexpr Point3D& operator=(Point3D&&) = default;

    ~Point3D() = default;

    // arithemtic operators (compound)
    constexpr Point3D& operator+=(const value_type) noexcept;
    constexpr Point3D& operator+=(const Point3D&) noexcept;
    constexpr Point3D& operator-=(const value_type) noexcept;
    constexpr Point3D& operator-=(const Point3D&) noexcept;
    constexpr Point3D& operator*=(const value_type) noexcept;
    constexpr Point3D& operator*=(const Point3D&) noexcept;
    constexpr Point3D& operator/=(const value_type) noexcept;
    constexpr Point3D& operator/=(const Point3D&) noexcept;

    // add a value to const point
    friend constexpr Point3D operator+(const Point3D& lhs, const value_type rhs)
        noexcept { return Point3D{lhs} += rhs; }
    // add a value to rvalue point
    friend constexpr Point3D operator+(Point3D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs += rhs); }
    // sum of two consts points
    friend constexpr Point3D operator+(const Point3D& lhs, const Point3D& rhs)
        noexcept { return Point3D{lhs} += rhs; }
    // sum of rvalue point and const point
    friend constexpr Point3D operator+(Point3D&& lhs, const Point3D& rhs)
        noexcept { return std::move(lhs += rhs); }
    // sum of two rvalue points
    friend constexpr Point3D operator+(Point3D&& lhs, Point3D&& rhs)
        noexcept { return std::move(lhs += rhs); }
    // rhs can be used as left-hand side since addition is commutative
    friend constexpr Point3D operator+(const value_type lhs, const Point3D& rhs)
        noexcept { return Point3D{rhs} += lhs; }
    friend constexpr Point3D operator+(const value_type lhs, Point3D&& rhs)
        noexcept { return std::move(rhs += lhs); }

    // subtract a value from const point
    friend constexpr Point3D operator-(const Point3D& lhs, const value_type rhs)
        noexcept { return Point3D{lhs} -= rhs; }
    // subtract a value from rvalue point
    friend constexpr Point3D operator-(Point3D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs -= rhs); }
    // difference of two consts points
    friend constexpr Point3D operator-(const Point3D& lhs, const Point3D& rhs)
        noexcept { return Point3D{lhs} -= rhs; }
    // difference of rvalue point and const point
    friend constexpr Point3D operator-(Point3D&& lhs, const Point3D& rhs)
        noexcept { return std::move(lhs -= rhs); }
    // difference of two rvalue points
    friend constexpr Point3D operator-(Point3D&& lhs, Point3D&& rhs)
        noexcept { return std::move(lhs -= rhs); }
    // create a new point since subtraction is not commutative
    friend constexpr Point3D operator-(const value_type lhs, const Point3D& rhs)
        noexcept { return Point3D{lhs, lhs, lhs} - rhs; }
    friend constexpr Point3D operator-(const value_type lhs, Point3D&& rhs)
        noexcept { return Point3D{lhs, lhs, lhs} - rhs; }

    // multiply const point by a value
    friend constexpr Point3D operator*(const Point3D& lhs, const value_type rhs)
        noexcept { return Point3D{lhs} *= rhs; }
    // multiply rvalue point by a value
    friend constexpr Point3D operator*(Point3D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs *= rhs); }
    // product of two consts points
    friend constexpr Point3D operator*(const Point3D& lhs, const Point3D& rhs)
        noexcept { return Point3D{lhs} *= rhs; }
    // product of rvalue point and const point
    friend constexpr Point3D operator*(Point3D&& lhs, const Point3D& rhs)
        noexcept { return std::move(lhs *= rhs); }
    // product of two rvalue points
    friend constexpr Point3D operator*(Point3D&& lhs, Point3D&& rhs)
        noexcept { return std::move(lhs *= rhs); }
    // rhs can be used as left-hand side since multiplication is commutative
    friend constexpr Point3D operator*(const value_type lhs, const Point3D& rhs)
        noexcept { return Point3D{rhs} *= lhs; }
    friend constexpr Point3D operator*(const value_type lhs, Point3D&& rhs)
        noexcept { return std::move(rhs *= lhs); }

    // divide const point by a value
    friend constexpr Point3D operator/(const Point3D& lhs, const value_type rhs)
        noexcept { return Point3D{lhs} /= rhs; }
    // divide rvalue point by a value
    friend constexpr Point3D operator/(Point3D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs /= rhs); }
    // quotient of two consts points
    friend constexpr Point3D operator/(const Point3D& lhs, const Point3D& rhs)
        noexcept { return Point3D{lhs} /= rhs; }
    // quotient of rvalue point and const point
    friend constexpr Point3D operator/(Point3D&& lhs, const Point3D& rhs)
        noexcept { return std::move(lhs /= rhs); }
    // quotient of two rvalue points
    friend constexpr Point3D operator/(Point3D&& lhs, Point3D&& rhs)
        noexcept { return std::move(lhs /= rhs); }
    // create a new point since division is not commutative
    friend constexpr Point3D operator/(const value_type lhs, const Point3D& rhs)
        noexcept { return Point3D{lhs, lhs, lhs} / rhs; }
    friend constexpr Point3D operator/(const value_type lhs, Point3D&& rhs)
        noexcept { return Point3D{lhs, lhs, lhs} / rhs; }

    // sum, product and dot product (Point3D as a 3D vector)
    constexpr value_type sum()  const noexcept { return x_ + y_ + z_; }
    constexpr value_type prod() const noexcept { return x_ * y_ * z_; }
    constexpr value_type dot(const Point3D& p) const noexcept {
        return x_ * p.x_ + y_ * p.y_ + z_ * p.z_;
    }
    // cross product
    constexpr Point3D cross(const Point3D &p) const noexcept {
        return Point3D { y_ * p.z_ - z_ * p.y_, z_ * p.x_ - x_ * p.z_,
            x_ * p.y_ - y_ * p.x_ };
    }

    // distance without squared root: can be used for comparison
    constexpr value_type squared_dist_to(const Point3D& p) const noexcept {
        return sqr(x_ - p.x_) + sqr(y_ - p.y_) + sqr(z_ - p.z_);
    }

    // Euclidean distance to another point
    constexpr double dist_to(const Point3D& p) const noexcept {
        return std::sqrt(squared_dist_to(p));
    }

    // norm of a 3D vector
    constexpr double squared_norm() const noexcept {
        return x_ * x_ + y_ * y_ + z_ * z_;
    }
    constexpr double norm() const noexcept { return std::sqrt(squared_norm()); }
    constexpr Point3D& normalize() noexcept { return (*this) /= norm(); }

    constexpr value_type& x() noexcept { return x_; }
    constexpr value_type x() const noexcept { return x_; }
    constexpr value_type& y() noexcept { return y_; }
    constexpr value_type y() const noexcept { return y_; }
    constexpr value_type& z() noexcept { return z_; }
    constexpr value_type z() const noexcept { return z_; }

private:
    value_type x_, y_, z_;

    // 'friendly' output/input operators
    friend std::ostream& operator<<(std::ostream& os, const Point3D& p) {
        return os << '(' << p.x_ << ", " << p.y_ << ", " << p.z_ << ')';
    }
    friend std::istream& operator>>(std::istream& is, Point3D& p) {
        return is >> p.x_ >> p.y_ >> p.z_;
    }
};

// constructors
template <class T>
constexpr Point3D<T>::Point3D() noexcept: x_{0}, y_{0}, z_{0} { }

template <class T>
constexpr Point3D<T>::Point3D(const value_type x, const value_type y,
        const value_type z) noexcept:
    x_ {x}, y_ {y}, z_{z} {
    }

// ------------------ arithmetic operators ------------------
template <class T>
constexpr Point3D<T>& Point3D<T>::operator+=(const value_type rhs) noexcept {
    x_ += rhs; y_ += rhs; z_ += rhs; return *this;
}

template <class T>
constexpr Point3D<T>& Point3D<T>::operator+=(const Point3D& rhs) noexcept {
    x_ += rhs.x_; y_ += rhs.y_; z_ += rhs.z_; return *this;
}

template <class T>
constexpr Point3D<T>& Point3D<T>::operator-=(const value_type rhs) noexcept {
    x_ -= rhs; y_ -= rhs; z_ -= rhs; return *this;
}

template <class T>
constexpr Point3D<T>& Point3D<T>::operator-=(const Point3D& rhs) noexcept {
    x_ -= rhs.x_; y_ -= rhs.y_; z_ -= rhs.z_; return *this;
}

template <class T>
constexpr Point3D<T>& Point3D<T>::operator*=(const value_type rhs) noexcept {
    x_ *= rhs; y_ *= rhs; z_ *= rhs; return *this;
}

template <class T>
constexpr Point3D<T>& Point3D<T>::operator*=(const Point3D& rhs) noexcept {
    x_ *= rhs.x_; y_ *= rhs.y_; z_ *= rhs.z_; return *this;
}

template <class T>
constexpr Point3D<T>& Point3D<T>::operator/=(const value_type rhs) noexcept {
    x_ /= rhs; y_ /= rhs; z_ /= rhs; return *this;
}

template <class T>
constexpr Point3D<T>& Point3D<T>::operator/=(const Point3D& rhs) noexcept {
    x_ /= rhs.x_; y_ /= rhs.y_; z_ /= rhs.z_; return *this;
}

// normalize a 3D vector given as Point3D
template <class T>
Point3D<T> normalize(const Point3D<T>& p) { return p / p.norm(); }


} // log_measurement namespace

#endif /* POINT3D_H */

