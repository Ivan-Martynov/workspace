#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <algorithm>
#include <atomic>
#include <cmath>

class Point {
public:
    constexpr Point(const double x_val = 0, const double y_val = 0) noexcept:
        x_ (x_val), y_ (y_val) {}

    constexpr Point(const Point&);
    Point& operator=(const Point&);
    Point(Point&&) = default;
    Point& operator=(Point&&) = default;

    constexpr double x() const noexcept { return x_; }
    constexpr double y() const noexcept { return y_; }

    // constexpr setters allowed in C++14
    constexpr void set_x(const double x) noexcept { x_ = x; }
    constexpr void set_y(const double y) noexcept { y_ = y; }

    double dist_from_origin() const noexcept;

private:
    mutable std::atomic<unsigned> call_count {0};
    double x_, y_;
};

constexpr Point::Point(const Point& p): x_ {p.x_}, y_ {p.y_} {}
Point& Point::operator=(const Point& p) {
    x_ = p.x_;
    y_ = p.y_;
    return *this;
}

double Point::dist_from_origin() const noexcept {
    ++call_count;
    return std::sqrt(x_ * x_ + y_ * y_);
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << '(' << p.x() << ", " << p.y() << ')';
}

#endif /* POINT_H */

