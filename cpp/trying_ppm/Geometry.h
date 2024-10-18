#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <cmath>

#include "PPM_Image.h"

class Shape
{
  public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual void draw(PPM_Image&, const PPM_Color&) const = 0;
    virtual void draw(PPM_Image&, const Color_name) const = 0;
};

class Point : public Shape
{
  public:
    Point();
    Point(const int, const int);
    Point(const Point&);
    Point& operator=(const Point&);

    int x() const
    {
        return x_;
    }
    int y() const
    {
        return y_;
    }

    double area() const override
    {
        return 0.0;
    }
    void draw(PPM_Image&, const PPM_Color&) const override;
    void draw(PPM_Image&, const Color_name = Color_name::white) const override;

  private:
    int x_;
    int y_;
};

constexpr double sqr(const int x)
{
    return x * x;
}

static double dist(const Point& p1, const Point& p2)
{
    return std::sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
}

class Line : public Shape
{
  public:
    Line(const int, const int, const int, const int);
    Line(const Point&, const Point&);
    Line(const Line&);
    Line& operator=(const Line&);

    double length() const
    {
        return dist(p1_, p2_);
    }
    double area() const override
    {
        return 0.0;
    }
    void draw(PPM_Image&, const PPM_Color&) const override;
    void draw(PPM_Image&, const Color_name = Color_name::white) const override;

  private:
    Point p1_;
    Point p2_;
};

#endif
