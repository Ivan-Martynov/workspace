#ifndef POINT2D_H
#define POINT2D_H

#include <fstream>
#include <algorithm>
#include "Utils.h"

namespace log_measurement {

// class for points in plane, represented by x and y coordinates
template <class T>
class Point2D {
    static_assert(std::is_arithmetic<T>::value, "Type should be arithmetic");
public:
    using value_type = T;

    constexpr Point2D() noexcept;
    explicit constexpr Point2D(const value_type, const value_type) noexcept;

    constexpr Point2D(const Point2D&) = default;
    constexpr Point2D& operator=(const Point2D&) = default;
    constexpr Point2D(Point2D&&) = default;
    constexpr Point2D& operator=(Point2D&&) = default;

    ~Point2D() = default;

    // arithemtic operators (compound)
    constexpr Point2D& operator+=(const value_type) noexcept;
    constexpr Point2D& operator+=(const Point2D&) noexcept;
    constexpr Point2D& operator-=(const value_type) noexcept;
    constexpr Point2D& operator-=(const Point2D&) noexcept;
    constexpr Point2D& operator*=(const value_type) noexcept;
    constexpr Point2D& operator*=(const Point2D&) noexcept;
    constexpr Point2D& operator/=(const value_type) noexcept;
    constexpr Point2D& operator/=(const Point2D&) noexcept;

    // add a value to const point
    friend constexpr Point2D operator+(const Point2D& lhs, const value_type rhs)
        noexcept { return Point2D{lhs} += rhs; }
    // add a value to rvalue point
    friend constexpr Point2D operator+(Point2D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs += rhs); }
    // sum of two consts points
    friend constexpr Point2D operator+(const Point2D& lhs, const Point2D& rhs)
        noexcept { return Point2D{lhs} += rhs; }
    // sum of rvalue point and const point
    friend constexpr Point2D operator+(Point2D&& lhs, const Point2D& rhs)
        noexcept { return std::move(lhs += rhs); }
    // sum of two rvalue points
    friend constexpr Point2D operator+(Point2D&& lhs, Point2D&& rhs)
        noexcept { return std::move(lhs += rhs); }
    // rhs can be used as left-hand side since addition is commutative
    friend constexpr Point2D operator+(const value_type lhs, const Point2D& rhs)
        noexcept { return Point2D{rhs} += lhs; }
    friend constexpr Point2D operator+(const value_type lhs, Point2D&& rhs)
        noexcept { return std::move(rhs += lhs); }

    // subtract a value from const point
    friend constexpr Point2D operator-(const Point2D& lhs, const value_type rhs)
        noexcept { return Point2D{lhs} -= rhs; }
    // subtract a value from rvalue point
    friend constexpr Point2D operator-(Point2D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs -= rhs); }
    // difference of two consts points
    friend constexpr Point2D operator-(const Point2D& lhs, const Point2D& rhs)
        noexcept { return Point2D{lhs} -= rhs; }
    // difference of rvalue point and const point
    friend constexpr Point2D operator-(Point2D&& lhs, const Point2D& rhs)
        noexcept { return std::move(lhs -= rhs); }
    // difference of two rvalue points
    friend constexpr Point2D operator-(Point2D&& lhs, Point2D&& rhs)
        noexcept { return std::move(lhs -= rhs); }
    // create a new point since subtraction is not commutative
    friend constexpr Point2D operator-(const value_type lhs, const Point2D& rhs)
        noexcept { return Point2D{lhs, lhs} - rhs; }
    friend constexpr Point2D operator-(const value_type lhs, Point2D&& rhs)
        noexcept { return Point2D{lhs, lhs} - rhs; }

    // multiply const point by a value
    friend constexpr Point2D operator*(const Point2D& lhs, const value_type rhs)
        noexcept { return Point2D{lhs} *= rhs; }
    // multiply rvalue point by a value
    friend constexpr Point2D operator*(Point2D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs *= rhs); }
    // product of two consts points
    friend constexpr Point2D operator*(const Point2D& lhs, const Point2D& rhs)
        noexcept { return Point2D{lhs} *= rhs; }
    // product of rvalue point and const point
    friend constexpr Point2D operator*(Point2D&& lhs, const Point2D& rhs)
        noexcept { return std::move(lhs *= rhs); }
    // product of two rvalue points
    friend constexpr Point2D operator*(Point2D&& lhs, Point2D&& rhs)
        noexcept { return std::move(lhs *= rhs); }
    // rhs can be used as left-hand side since multiplication is commutative
    friend constexpr Point2D operator*(const value_type lhs, const Point2D& rhs)
        noexcept { return Point2D{rhs} *= lhs; }
    friend constexpr Point2D operator*(const value_type lhs, Point2D&& rhs)
        noexcept { return std::move(rhs *= lhs); }

    // divide const point by a value
    friend constexpr Point2D operator/(const Point2D& lhs, const value_type rhs)
        noexcept { return Point2D{lhs} /= rhs; }
    // divide rvalue point by a value
    friend constexpr Point2D operator/(Point2D&& lhs, const value_type rhs)
        noexcept { return std::move(lhs /= rhs); }
    // quotient of two consts points
    friend constexpr Point2D operator/(const Point2D& lhs, const Point2D& rhs)
        noexcept { return Point2D{lhs} /= rhs; }
    // quotient of rvalue point and const point
    friend constexpr Point2D operator/(Point2D&& lhs, const Point2D& rhs)
        noexcept { return std::move(lhs /= rhs); }
    // quotient of two rvalue points
    friend constexpr Point2D operator/(Point2D&& lhs, Point2D&& rhs)
        noexcept { return std::move(lhs /= rhs); }
    // create a new point since division is not commutative
    friend constexpr Point2D operator/(const value_type lhs, const Point2D& rhs)
        noexcept { return Point2D{lhs, lhs} / rhs; }
    friend constexpr Point2D operator/(const value_type lhs, Point2D&& rhs)
        noexcept { return Point2D{lhs, lhs} / rhs; }

    // sum, product and dot product (Point2D as a 2D vector)
    constexpr value_type sum()  const noexcept { return x_ + y_; }
    constexpr value_type prod() const noexcept { return x_ * y_; }
    constexpr value_type dot(const Point2D& p) const noexcept {
        return x_ * p.x_ + y_ * p.y_;
    }

    // distance without squared root: can be used for comparison
    constexpr value_type squared_dist_to(const Point2D& p) const noexcept {
        return sqr(x_ - p.x_) + sqr(y_ - p.y_);
    }

    // Euclidean distance to another point
    constexpr double dist_to(const Point2D& p) const noexcept {
        return std::sqrt(squared_dist_to(p));
    }

    // norm of a 2D vector
    constexpr double squared_norm() const noexcept { return x_ * x_ + y_ * y_; }
    constexpr double norm() const noexcept { return std::sqrt(squared_norm()); }
    constexpr Point2D& normalize() noexcept { return (*this) /= norm(); }

    constexpr value_type& x() noexcept { return x_; }
    constexpr value_type x() const noexcept { return x_; }
    constexpr value_type& y() noexcept { return y_; }
    constexpr value_type y() const noexcept { return y_; }

    // swapping
    void swap(Point2D& o) noexcept { std::swap(x_, o.x_); std::swap(y_, o.y_); }

private:
    value_type x_, y_;

    // 'friendly' output/input operators
    friend std::ostream& operator<<(std::ostream& os, const Point2D& p) {
        return os << '(' << p.x_ << ", " << p.y_ << ')';
    }
    friend std::istream& operator>>(std::istream& is, Point2D& p) {
        return is >> p.x_ >> p.y_;
    }
};

// constructors
template <class T>
constexpr Point2D<T>::Point2D() noexcept: x_{0}, y_{0} { }

template <class T>
constexpr Point2D<T>::Point2D(const value_type x, const value_type y) noexcept:
    x_ {x}, y_ {y} {
    }

// ------------------ arithmetic operators ------------------
template <class T>
constexpr Point2D<T>& Point2D<T>::operator+=(const value_type rhs) noexcept {
    x_ += rhs; y_ += rhs; return *this;
}

template <class T>
constexpr Point2D<T>& Point2D<T>::operator+=(const Point2D& rhs) noexcept {
    x_ += rhs.x_; y_ += rhs.y_; return *this;
}

template <class T>
constexpr Point2D<T>& Point2D<T>::operator-=(const value_type rhs) noexcept {
    x_ -= rhs; y_ -= rhs; return *this;
}

template <class T>
constexpr Point2D<T>& Point2D<T>::operator-=(const Point2D& rhs) noexcept {
    x_ -= rhs.x_; y_ -= rhs.y_; return *this;
}

template <class T>
constexpr Point2D<T>& Point2D<T>::operator*=(const value_type rhs) noexcept {
    x_ *= rhs; y_ *= rhs; return *this;
}

template <class T>
constexpr Point2D<T>& Point2D<T>::operator*=(const Point2D& rhs) noexcept {
    x_ *= rhs.x_; y_ *= rhs.y_; return *this;
}

template <class T>
constexpr Point2D<T>& Point2D<T>::operator/=(const value_type rhs) noexcept {
    x_ /= rhs; y_ /= rhs; return *this;
}

template <class T>
constexpr Point2D<T>& Point2D<T>::operator/=(const Point2D& rhs) noexcept {
    x_ /= rhs.x_; y_ /= rhs.y_; return *this;
}

// normalize a 2D vector given as Point2D
template <class T>
Point2D<T> normalize(const Point2D<T>& p) { return p / p.norm(); }

// swap non-member function
template <class T>
void swap(Point2D<T>& lhs, Point2D<T>& rhs) noexcept { lhs.swap(rhs); }

} // log_measurement namespace

#endif // POINT2D_H

