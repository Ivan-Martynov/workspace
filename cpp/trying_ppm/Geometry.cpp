#include "Geometry.h"

Point::Point(): x_{0}, y_{0} { }

Point::Point(const int xx, const int yy): x_{xx}, y_{yy} { }

Point::Point(const Point &o): x_{o.x()}, y_{o.y()} { }

Point &Point::operator=(const Point &o) {
    if (this != &o) {
        x_ = o.x();
        y_ = o.y();
    }
    return *this;
}

void Point::draw(PPM_Image &img, const Color_name cn) const {
    img.pixel_color(x_, y_, PPM_Color{cn});
}

void Point::draw(PPM_Image &img, const PPM_Color &c) const {
    img.pixel_color(x_, y_, c);
}

