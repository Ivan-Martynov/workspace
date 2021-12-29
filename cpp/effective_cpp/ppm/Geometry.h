/*
 * Geometry classes:
 * Using OOP techniques to implement geometrical primitives (points, lines,
 * triangles).
 * Shape is a virtual base class and all other classes are derived from it.
 * The Point here is implemented outside the Shape class for practice and test
 * (with probable addition of rotation around a point method)
 *
 * Shape declares methods for length (or perimeter), area of a shape,
 * drawing and filling shapes. Obviously area of a line is zero but I have
 * decided to keep it in the implementation in order not to remove it from
 * the Shape class. After all, it does not harm so far and might be useful
 * later... The same concerns the filling of lines (simply copy drawing, since
 * filling only makes sense for closed polygons: in our case, triangles only)
 * Added:
 *      - Cohen–Sutherland line clipping algorithm: clip the line so that the
 *      coordinates beyond image are not processed
 *      - Circle class
 *      - Half-space filling algorithm for triangle
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <vector>

#include "PPM_Image.h"

namespace Geometry_lib { // general namespace for this small library

// pi constant
static constexpr double pi {std::acos(-1)};

// function to compute a square value: x^2
template <class T>
constexpr T sqr(const T &val) { return val * val; }

/*
 * ------------------ Range_error struct ------------------
 */
struct Range_error: std::out_of_range {
    size_t idx;
    Range_error(const size_t i):
        std::out_of_range{"Range error: index " + std::to_string(i)}, idx{i} {
    }
};

class Shape {
protected:
    Shape() = default;
    virtual ~Shape() = default;
public:
    using RGB_Image = PPM_lib::RGB_Image;
    using RGB_Color = PPM_lib::RGB_Color;

    virtual double length() const noexcept { return 0; } // length, perimeter...
    virtual double area() const noexcept { return 0; }
    void draw(RGB_Image &I, const RGB_Color &c = RGB_Color{}) const {
        doDraw(I, c);
    }
    void fill(RGB_Image &I, const RGB_Color &c = RGB_Color{}) const {
        doFill(I, c);
    }
private:
    virtual void doDraw(RGB_Image&, const RGB_Color&) const = 0;
    virtual void doFill(RGB_Image&, const RGB_Color&) const = 0;
};

class Point: public Shape {
public:
    Point() noexcept: x_{0}, y_{0} { }
    explicit Point(const int xx, const int yy) noexcept: x_{xx}, y_{yy} { }
    explicit Point(const double xd, const double yd) noexcept: x_(xd), y_(yd){}
    Point(const Point &o): x_{o.getX()}, y_{o.getY()} { }
    Point& operator=(const Point&);
    Point(Point&& o) noexcept: x_{std::move(o.x_)}, y_{std::move(o.y_)} { }
    Point& operator=(Point&& o) noexcept {
        x_ = std::move(o.x_);
        y_ = std::move(o.y_);
        return  *this;
    }
    ~Point() { };

    size_t size() const { return 2; }

    int& operator[](const size_t i) { return (i < 1) ? x_ : y_; }
    const int& operator[](const size_t i) const { return (i < 1) ? x_ : y_; }
    int& at(const size_t i) {
        return (i < size()) ? operator[](i) :
            (throw Range_error(i), operator[](i));
    }
    const int& at(const size_t i) const {
        return (i < size()) ? operator[](i) :
            (throw Range_error(i), operator[](i));
    }

    int getX() const { return x_; }
    int getY() const { return y_; }
    int x() const noexcept { return x_; }
    int y() const noexcept { return y_; }
    int& x() noexcept { return x_; }
    int& y() noexcept { return y_; }

    double dist_to(const Point &o) const noexcept {
        return std::sqrt(sqr(x_ - o.x_) + sqr(y_ - o.y_));
    }

private:
    int x_;
    int y_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

//class Point_array: public Shape {
//public:
//    Point_array();
//    Point_array(const std::vector<Point>&);
//    Point_array(const std::initializer_list<Point>&);
//    Point_array(const Point_array&);
//    Point_array& operator=(const Point_array&);
//    Point_array(Point_array&&);
//    Point_array& operator=(Point_array&&);
//    ~Point_array() { };
//
//    using iterator = typename std::vector<Point>::iterator;
//    using const_iterator = typename std::vector<Point>::const_iterator;
//
//    iterator begin() { return pa_.begin(); }
//    const_iterator begin() const { return pa_.begin(); }
//    const_iterator cbegin() const { return pa_.cbegin(); }
//    iterator end() { return pa_.end(); }
//    const_iterator end() const { return pa_.end(); }
//    const_iterator cend() const { return pa_.cend(); }
//
//    iterator insert(iterator pos, const Point &p) {
//        return pa_.insert(pos, p);
//    }
//    iterator erase(iterator pos) { return pa_.erase(pos); }
//    iterator erase(iterator f, iterator b) { return pa_.erase(f, b); }
//    void push_back(const Point &p) { pa_.push_back(p); }
//    void push_back(Point&& p) { pa_.push_back(p); }
//
//    Point& operator[](const size_t i) { return pa_[i]; }
//    const Point& operator[](const size_t i) const { return pa_[i]; }
//    Point& at(const size_t i) { return pa_.at(i); }
//    const Point& at(const size_t i) const { return pa_.at(i); }
//
//    size_t size() const { return pa_.size(); }
//    const std::vector<Point>& points() const { return pa_; }
//
//protected:
//    std::vector<Point> pa_;
//    //virtual void doDraw(PPM_Image&, const PPM_Color&) const override;
//    //virtual void doFill(PPM_Image&, const PPM_Color&) const override;
//};

class Line: public Shape {
public:
    explicit Line(const Point &p1, const Point &p2) noexcept:
        p1_{p1}, p2_{p2} { }
    explicit Line(const Point &pp, const int xx, const int yy) noexcept:
        p1_{pp}, p2_{Point{xx, yy}} { }
    explicit Line(const int x1, const int y1, const int x2, const int y2)
        noexcept: p1_{Point{x1, y1}}, p2_{Point{x2, y2}} { }
    Line(const Line &o): p1_{o.p1_}, p2_{o.p2_} { }
    Line& operator=(const Line&);
    ~Line() = default;

    double length() const noexcept override { return p1_.dist_to(p2_); }

private:
    Point p1_;
    Point p2_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Triangle: public Shape {
public:
    Triangle(const Point&, const Point&, const Point&);
    Triangle(const int, const int, const int, const int, const int, const int);
    Triangle(const Triangle&);
    Triangle& operator=(const Triangle&);
    ~Triangle() = default;

    double length() const noexcept override;
    double area() const noexcept override;

    void fill_hs(RGB_Image&, const RGB_Color& = RGB_Color{}) const;
    void fill_bary(RGB_Image&, const RGB_Color& = RGB_Color{}) const;

    Point& p1() { return p1_; }
    const Point& p1() const { return p1_; }
    Point& p2() { return p2_; }
    const Point& p2() const { return p2_; }
    Point& p3() { return p3_; }
    const Point& p3() const { return p3_; }

private:
    Point p1_;
    Point p2_;
    Point p3_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};



}

#endif

