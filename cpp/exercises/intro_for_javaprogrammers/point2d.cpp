#include "point2d.h"

Point2D::Point2D(double x, double y) : xVal {x}, yVal {y} {}

Point2D::Point2D() : Point2D(0.0, 0.0) {}

void Point2D::setX(double x)
{
    xVal = x;
}

void Point2D::setY(double y)
{
    yVal = y;
}

double Point2D::getX() const
{
    return xVal;
}

double Point2D::getY() const
{
    return yVal;
}
