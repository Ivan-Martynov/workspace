#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <algorithm>
#include "PPM_Image.h"

template <class T>
constexpr T sqr_fun(const T val) {
    return val * val;
}

class Point {
public:
    Point();
    Point(const int, const int);
    Point(const Point&);
    Point &operator=(const Point&);

    ~Point() = default;

    int x() const { return x_; }
    int y() const { return y_; }

    double dist_to(const Point &o) const { return sqrt(sqr_fun(x_ - o.x_) +
            sqr_fun(y_ - o.y_)); }
    void draw(PPM_Image&, const PPM_Color& = PPM_Color{255}) const;

private:
    int x_;
    int y_;
};

class Shape {
public:
    virtual double length() const = 0; // length, perimeter...
    virtual double area() const = 0;
    virtual void draw(PPM_Image&, const PPM_Color&) const = 0;
    virtual void fill(PPM_Image&, const PPM_Color&) const = 0;
};

class Line: public Shape {
public:
    Line(const Point&, const Point&);
    Line(const Point&, const int, const int);
    Line(const int, const int, const int, const int);
    Line(const Line&);
    Line &operator=(const Line&);

    ~Line() = default;

    double length() const override { return p1_.dist_to(p2_); }
    double area() const override { return 0.0; }
    void draw(PPM_Image&, const PPM_Color& = PPM_Color{255}) const override;
    void fill(PPM_Image&, const PPM_Color& = PPM_Color{255}) const override;

private:
    Point p1_;
    Point p2_;
};

class Triangle: public Shape {
public:
    Triangle(const Point&, const Point&, const Point&);
    Triangle(const int, const int, const int, const int, const int, const int);
    Triangle(const Triangle&);
    Triangle &operator=(const Triangle&);

    ~Triangle() = default;

    double length() const override;
    double area() const override;
    void draw(PPM_Image&, const PPM_Color& = PPM_Color{255}) const override;
    void fill(PPM_Image&, const PPM_Color& = PPM_Color{255}) const override;

private:
    Point p1_;
    Point p2_;
    Point p3_;
};

#endif

