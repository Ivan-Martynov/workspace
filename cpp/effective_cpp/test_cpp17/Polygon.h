#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "Point2D.h"

namespace log_measurement {

/*
 * Polygon of two-dimensional points
 */
template <class T>
class Polygon {
public:
    using value_type       = Point2D<T>;
    using size_type        = typename std::vector<value_type>::size_type;
    using iterator         = typename std::vector<value_type>::iterator;
    using reverse_iterator = typename std::vector<value_type>::reverse_iterator;
    using const_iterator   = typename std::vector<value_type>::const_iterator;
    using const_reverse_iterator =
        typename std::vector<value_type>::const_reverse_iterator;

    // default constructor
    Polygon() noexcept: pts_{} { };
    // constructor copying const vector
    explicit Polygon(const std::vector<value_type>& v) noexcept: pts_{v} { };
    // constructor moving vector
    explicit Polygon(std::vector<value_type>&& v) noexcept: pts_{std::move(v)}
    { };

    Polygon(const Polygon& o): pts_{o.pts_} { }
    Polygon& operator=(const Polygon& o) {
        if (this != &o) pts_ = o.pts_;
	return *this;
    }
    Polygon(Polygon&& o) noexcept: pts_{std::move(o.pts_)} { }
    Polygon& operator=(Polygon&& o) { pts_ = std::move(o.pts_); return *this; }

    ~Polygon() = default;

    /*
     * ------------ Iterators and access ------------
     */
    // begin iterators
    iterator begin() noexcept { return pts_.begin(); }
    const_iterator begin() const noexcept { return pts_.begin(); }
    const_iterator cbegin() const noexcept { return pts_.cbegin(); }

    // end iterators
    iterator end() noexcept { return pts_.end(); }
    const_iterator end() const noexcept { return pts_.end(); }
    const_iterator cend() const noexcept { return pts_.cend(); }

    // reverse begin iterators
    reverse_iterator rbegin() noexcept { return pts_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return pts_.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return pts_.crbegin(); }

    // reverse end iterators
    reverse_iterator rend() noexcept { return pts_.rend(); }
    const_reverse_iterator rend() const noexcept { return pts_.rend(); }
    const_reverse_iterator crend() const noexcept { return pts_.crend(); }

    // provide access to the first and last points
    value_type& front() { return pts_.front(); }
    const value_type& front() const { return pts_.front(); }
    value_type& back() { return pts_.back(); }
    const value_type& back() const { return pts_.back(); }

    // index access
    value_type& operator[](const size_t i) { return pts_[i]; }
    const value_type& operator[](const size_t i) const { return pts_[i]; }
    value_type& at(const size_t i) { return pts_.at(i); }
    const value_type& at(const size_t i) const { return pts_.at(i); }
    /*
     * ------------ End Iterators and access ------------
     */

    // checking if the polygon is empty
    bool is_empty() const noexcept { return pts_.empty(); }

    // number of points
    size_type size() const noexcept { return pts_.size(); }

    // clear points
    void clear() noexcept { pts_.clear(); }

    // add element to the end
    void push_back(const value_type& p) { pts_.push_back(p); }
    void push_back(value_type&& p) { pts_.push_back(p); }

    // emplace to the end
    template <class... Args>
        void emplace_back(Args&&... args) { pts_.emplace_back(args...); }

    // swapping
    void swap(Polygon& o) noexcept { pts_.swap(o.pts_); }

    // perimeter of a polygon
    double perimeter() const noexcept;

    // signed area of a polygon
    double signed_area() const noexcept;
    // polygon area
    double area() const noexcept { return std::abs(signed_area()); }

private:
    std::vector<Point2D<T>> pts_; // polygon points
};

// perimeter of a polygon
template <class T>
double Polygon<T>::perimeter() const noexcept {
    // remember the number of points in the polygon
    const auto num_points = size();

    // polygon should have at least two points to have a non-zero perimeter
    if (num_points < 2)
        return 0.0;

    // initialize perimeter with the distance between the last and first points
    auto res = back().dist_to(front());

    // add pairwise distances
    for (size_type i {0}; i < num_points - 1; ++i)
        res += pts_[i].dist_to(pts_[i + 1]);

    return res;
}

// signed area of a polygon
template <class T>
double Polygon<T>::signed_area() const noexcept {
    // remember the number of points in the polygon
    const auto num_points = size();

    // polygon should have at least three points to have a non-zero area
    if (num_points < 3)
        return 0.0;

    // initialize the area value
    auto signed_area = front().x() * (front().y() - back().y()) +
        back().x() * (front().y() - pts_[num_points - 2].y());

    // loop through the points
    for (size_t i {1}; i < num_points - 1; ++i)
        signed_area += pts_[i].x() * (pts_[i + 1].y() - pts_[i - 1].y());
    return signed_area * 0.5;
}

// swap non-member function
template <class T>
void swap(Polygon<T>& lhs, Polygon<T>& rhs) noexcept { lhs.swap(rhs); }

} // log_measurement namespace

#endif /* POLYGON_H */

