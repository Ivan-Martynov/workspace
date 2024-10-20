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

#include "PPM_lib.h"
#include "Vec.h"
#include <algorithm>

#include <iostream>
#include <stdexcept>
#include <vector>

namespace PPM_lib
{ // general namespace for this small library

// pi constant
static constexpr double pi {std::acos(-1)};

// function to compute a square value: x^2
template <class T> constexpr T sqr(const T& val)
{
    return val * val;
}

/*
 * ------------------ Range_error struct ------------------
 */
struct Range_error : std::out_of_range
{
    size_t idx;
    Range_error(const size_t i)
        : std::out_of_range {"Range error: index " + std::to_string(i)}, idx {i}
    {
    }
};

class Point; // forward declaration

class Shape
{
  protected:
    Shape() = default;

  public:
    virtual ~Shape() = default;

    virtual double length() const
    {
        return 0;
    } // length, perimeter...
    virtual double area() const
    {
        return 0;
    }
    void draw(RGB_Image& I, const RGB_Color& c = 255) const
    {
        doDraw(I, c);
    }
    void fill(RGB_Image& I, const RGB_Color& c = 255) const
    {
        doFill(I, c);
    }

  private:
    virtual void doDraw(RGB_Image&, const RGB_Color&) const = 0;
    virtual void doFill(RGB_Image&, const RGB_Color&) const = 0;
};

class Point : public Shape
{
  public:
    constexpr Point() : x_ {0}, y_ {0} {}
    constexpr Point(const int xx, const int yy) : x_ {xx}, y_ {yy} {}
    constexpr Point(const double xd, const double yd) : x_(xd), y_(yd) {}
    Point(const Point& o) : x_ {o.getX()}, y_ {o.getY()} {}
    Point& operator=(const Point&);
    ~Point() = default;

    constexpr size_t size()
    {
        return 2;
    }

    int& operator[](const size_t i)
    {
        return (i < 1) ? x_ : y_;
    }
    const int& operator[](const size_t i) const
    {
        return (i < 1) ? x_ : y_;
    }
    int& at(const size_t i)
    {
        return (i < size()) ? operator[](i)
                            : (throw Range_error(i), operator[](i));
    }
    const int& at(const size_t i) const
    {
        return (i < size()) ? operator[](i)
                            : (throw Range_error(i), operator[](i));
    }

    constexpr int getX()
    {
        return x_;
    }
    constexpr int getY()
    {
        return y_;
    }
    const int& x() const
    {
        return x_;
    }
    const int& y() const
    {
        return y_;
    }
    int& x()
    {
        return x_;
    }
    int& y()
    {
        return y_;
    }

    constexpr double dist_to(const Point& o) const
    {
        return sqrt(sqr(x_ - o.x_) + sqr(y_ - o.y_));
    }

  private:
    int x_;
    int y_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Point_array : public Shape
{
  public:
    Point_array();
    Point_array(const std::vector<Point>&);
    Point_array(const std::initializer_list<Point>&);
    Point_array(const Point_array&);
    Point_array& operator=(const Point_array&);
    Point_array(Point_array&&);
    Point_array& operator=(Point_array&&);
    ~Point_array() = default;

    using iterator = typename std::vector<Point>::iterator;
    using const_iterator = typename std::vector<Point>::const_iterator;

    iterator begin()
    {
        return pa_.begin();
    }
    const_iterator begin() const
    {
        return pa_.begin();
    }
    const_iterator cbegin() const
    {
        return pa_.cbegin();
    }
    iterator end()
    {
        return pa_.end();
    }
    const_iterator end() const
    {
        return pa_.end();
    }
    const_iterator cend() const
    {
        return pa_.cend();
    }

    iterator insert(iterator pos, const Point& p)
    {
        return pa_.insert(pos, p);
    }
    iterator erase(iterator pos)
    {
        return pa_.erase(pos);
    }
    iterator erase(iterator f, iterator b)
    {
        return pa_.erase(f, b);
    }
    void push_back(const Point& p)
    {
        pa_.push_back(p);
    }
    void push_back(Point&& p)
    {
        pa_.push_back(p);
    }

    Point& operator[](const size_t i)
    {
        return pa_[i];
    }
    const Point& operator[](const size_t i) const
    {
        return pa_[i];
    }
    Point& at(const size_t i)
    {
        return pa_.at(i);
    }
    const Point& at(const size_t i) const
    {
        return pa_.at(i);
    }

    size_t size() const
    {
        return pa_.size();
    }
    const std::vector<Point>& points() const
    {
        return pa_;
    }

  protected:
    std::vector<Point> pa_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Line : public Shape
{
  public:
    constexpr Line(const Point& p1, const Point& p2) : p1_ {p1}, p2_ {p2} {}
    Line(const Point& pp, const int xx, const int yy)
        : p1_ {pp}, p2_ {Point {xx, yy}}
    {
    }
    Line(const int x1, const int y1, const int x2, const int y2)
        : p1_ {Point {x1, y1}}, p2_ {Point {x2, y2}}
    {
    }
    constexpr Line(const Line& o) : p1_ {o.p1_}, p2_ {o.p2_} {}
    Line& operator=(const Line&);
    ~Line() = default;

    double length() const override
    {
        return p1_.dist_to(p2_);
    }

  private:
    Point p1_;
    Point p2_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Rectangle : public Shape
{
  public:
    constexpr Rectangle(const Point& pp, const size_t ww, const size_t hh)
        : p_ {pp}, w_ {ww}, h_ {hh}
    {
    }
    Rectangle(
        const int xx, const int yy, const size_t ww, const size_t hh)
        : p_ {Point {xx, yy}}, w_ {ww}, h_ {hh}
    {
    }
    constexpr Rectangle(const Rectangle& o) : p_ {o.p_}, w_ {o.w_}, h_ {o.h_} {}
    Rectangle& operator=(const Rectangle&);
    ~Rectangle() = default;

    double length() const override
    {
        return (w_ + h_) << 1;
    }
    double area() const override
    {
        return w_ * h_;
    }

  private:
    Point p_;
    size_t w_;
    size_t h_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Polyline : public Point_array
{
  public:
    Polyline() : Point_array() {}
    Polyline(const std::vector<Point>& vp) : Point_array {vp} {}
    Polyline(const std::initializer_list<Point>& pil) : Point_array {pil} {}
    Polyline(const Polyline& o) : Point_array {o} {}
    Polyline(Polyline&& o) : Point_array {o} {}

    double length() const override;

  private:
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Triangle : public Shape
{
  public:
    Triangle(const Point&, const Point&, const Point&);
    Triangle(const int, const int, const int, const int, const int, const int);
    Triangle(const Triangle&);
    Triangle& operator=(const Triangle&);
    ~Triangle() = default;

    double length() const override;
    double area() const override;

    void fill_hs(RGB_Image&, const RGB_Color& = 255) const;
    void fill_bary(RGB_Image&, const RGB_Color& = 255) const;

  private:
    Point p1_;
    Point p2_;
    Point p3_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Polygon : public Polyline
{
  public:
    Polygon() : Polyline() {}
    Polygon(const std::vector<Point>& vp) : Polyline {vp} {}
    Polygon(const std::initializer_list<Point>& pil) : Polyline {pil} {}
    Polygon(const Polyline& o) : Polyline {o} {}
    Polygon(Polyline&& o) : Polyline {o} {}

    double length() const override;
    double area() const override;

  private:
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

class Circle : public Shape
{
  public:
    Circle(const Point&, const size_t);
    Circle(const int, const int, const size_t);
    Circle(const Circle&);
    Circle& operator=(const Circle&);
    ~Circle() = default;

    double length() const override
    {
        return pi * (r_ << 1);
    }
    double area() const override
    {
        return pi * sqr(r_);
    }

  private:
    Point p_;
    size_t r_;
    virtual void doDraw(RGB_Image&, const RGB_Color&) const override;
    virtual void doFill(RGB_Image&, const RGB_Color&) const override;
};

} // end namespace PPM_lib

#endif
