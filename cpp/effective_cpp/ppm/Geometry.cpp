#include "Geometry.h"

using namespace Geometry_lib;
using RGB_Image = PPM_lib::RGB_Image;
using RGB_Color = PPM_lib::RGB_Color;

/*
 * ------------------ Point implementation ------------------
 */
Point& Point::operator=(const Point &o) {
    if (this != &o) { x_ = o.getX(); y_ = o.getY(); } return *this;
}

void Point::doDraw(RGB_Image &I, const RGB_Color &c) const {
    if (x_ >= 0 && x_ < I.width() && y_ >= 0 && y_ < I.height())
        I(x_, y_) = c;
}

void Point::doFill(RGB_Image &I, const RGB_Color &c) const {
    Point::draw(I, c);
}

/*
 * ------------------ Line implementation ------------------
 */
Line& Line::operator=(const Line &o) {
    if (this != &o) {
        p1_ = o.p1_;
        p2_ = o.p2_;
    }
    return *this;
}

/*
 * computing code for Cohen-Sutherland algorithm
 * In our case the bottom is reached when the y coordinate is maximum: common
 * case for image coordinates
 */
int out_code(const int x, const int y, const int xmin, const int xmax,
        const int ymin, const int ymax) {
    static constexpr int inside {0}, left {1}, right {2}, bottom {4}, top {8};
    int code {inside};
    if (x < xmin) code |= left; else if (x >= xmax) code |= right;
    if (y < ymin) code |= top; else if (y >= ymax) code |= bottom;
    return code;
}

/*
 * Cohen–Sutherland line clipping algorithm
 * In our case the bottom is reached when the y coordinate is maximum: common
 * case for image coordinates
 * The coordinates are passed by reference, so that they might be returned with
 * changed values
 */
bool clip_line(int &x1, int &y1, int &x2, int &y2,
        const int xmin, const int xmax, const int ymin, const int ymax) {
    static constexpr int left {1}, right {2}, bottom {4}, top {8};
    int code1 {out_code(x1, y1, xmin, xmax, ymin, ymax)};
    int code2 {out_code(x2, y2, xmin, xmax, ymin, ymax)};
    while (true) {
        if (!(code1 | code2)) // both ends inside the box
            return true;
        else if (code1 & code2) // line is completely outside
            return false;
        else {
            const int code {code1 ? code1 : code2};
            double x {}, y {};
            if (code & top) {
                x = x1 + (x2 - x1) * double(ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (code & bottom) {
                x = x1 + (x2 - x1) * double(ymax - y1) / (y2 - y1);
                y = ymax - 1;
            } else if (code & right) {
                y = y1 + (y2 - y1) * double(xmax - x1) / (x2 - x1);
                x = xmax - 1;
            }
            else if (code & left) {
                y = y1 + (y2 - y1) * double(xmin - x1) / (x2 - x1);
                x = xmin;
            }
            if (code == code1) {
                x1 = x;
                y1 = y;
                code1 = out_code(x1, y1, xmin, xmax, ymin, ymax);
            } else {
                x2 = x;
                y2 = y;
                code2 = out_code(x2, y2, xmin, xmax, ymin, ymax);
            }
        }
    }
}

/*
 * Bresenham algorithm for drawing line points
 */
void Line::doDraw(RGB_Image &I, const RGB_Color &c) const {
    int x1 {p1_.x()}, y1 {p1_.y()}, x2 {p2_.x()}, y2 {p2_.y()};
    // clip the line if needed
    if (!clip_line(x1, y1, x2, y2, 0, I.width(), 0, I.height()))
        return;
    int dx {std::abs(x1- x2)}, dy {std::abs(y1 - y2)};
    const bool steep {dy > dx};
    if (steep) { std::swap(x1, y1); std::swap(x2, y2); std::swap(dx, dy); }
    if (x1 > x2) { std::swap(x1, x2); std::swap(y1, y2); }
    const int incdy {dy << 1}, incdx {dx << 1}, ystep {y1 < y2 ? 1 : -1};
    //const uint clr {c.color()};
    for (int x {x1}, e {dx}; x <= x2; ++x) {
        steep ? I(y1, x) = c : I(x, y1) = c;
        //steep ? I[y1][x] = clr : I[x][y1] = clr;
        if ((e -= incdy) < 0) {
            y1 += ystep;
            e += incdx;
        }
    }
}

void Line::doFill(RGB_Image &I, const RGB_Color &c) const {
    Line::draw(I, c);
}

/*
 * helper methods for drawing straight lines: simpler than an arbitrary line
 */
void draw_horizontal_line(RGB_Image &I, const RGB_Color &c,
        const int x1, const int x2, const int y) {
    //const uint clr {c.color()};
    for (auto x = x1; x <= x2; ++x)
        I(x, y) = c;
        //I[x][y] = clr;
}

void clip_n_draw_horizontal_line(RGB_Image &I, const RGB_Color &c,
        const int x1, const int x2, const int y) {
    const int w = I.width() - 1;
    draw_horizontal_line(I, c, std::min(std::max(0, x1), w),
            std::min(std::max(0, x2), w), y);
}

void draw_vertical_line(RGB_Image &I, const RGB_Color &c,
        const int x, const int y1, const int y2) {
    for (auto y = y1; y <= y2; ++y)
        I(x, y) = c;
}

void clip_n_draw_vertical_line(RGB_Image &I, const RGB_Color &c,
        const int x, const int y1, const int y2) {
    const int h = I.height() - 1;
    draw_vertical_line(I, c, x, std::min(std::max(0, y1), h),
            std::min(std::max(0, y2), h));
}

/*
 * ------------------ Triangle implementation ------------------
 */
Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3): p1_{p1},
    p2_{p2}, p3_{p3} {
    }

Triangle::Triangle(const int x1, const int y1, const int x2, const int y2,
        const int x3, const int y3): p1_{Point{x1, y1}}, p2_{Point{x2, y2}},
    p3_{Point{x3, y3}} {
    }

Triangle::Triangle(const Triangle &o): p1_{o.p1_}, p2_{o.p2_}, p3_{o.p3_} {
}

Triangle& Triangle::operator=(const Triangle &o) {
    if (this != &o) {
        p1_ = o.p1_;
        p2_ = o.p2_;
        p3_ = o.p3_;
    }
    return *this;
}

double Triangle::length() const noexcept {
    return p1_.dist_to(p2_) + p2_.dist_to(p3_) + p3_.dist_to(p1_);
}

double Triangle::area() const noexcept {
    return std::abs((p2_.x() - p1_.x()) * (p3_.y() - p1_.y()) -
            (p3_.x() - p1_.x()) * (p2_.y() - p1_.y())) * 0.5;
}

void Triangle::doDraw(RGB_Image &I, const RGB_Color &c) const {
    Line{p1_, p2_}.draw(I, c);
    Line{p2_, p3_}.draw(I, c);
    Line{p3_, p1_}.draw(I, c);
}

/*
 * standard filling algorithm
 */
void Triangle::doFill(RGB_Image &I, const RGB_Color &c) const {
    Point p1 {p1_}, p2 {p2_}, p3 {p3_};
    if (p1.y() == p2.y() && p2.y() == p3.y()) return;
    // sort the vertices
    if (p1.y() > p2.y()) std::swap(p1, p2);
    if (p1.y() > p3.y()) std::swap(p1, p3);
    if (p2.y() > p3.y()) std::swap(p2, p3);
    // auxilliary variables
    int y1 {p1.y()}, y2 {p2.y()}, y3 {p3.y()};
    const int x1 {p1.x()}, x2 {p2.x()}, x3 {p3.x()}, h {y3 - y1}; // height
    const int dx12 {x2- x1}, dx13 {x3 - x1}, dx23 {x3 - x2};
    const int dy12 {y2- y1}, dy23 {y3 - y2};
    const bool is_y12 {y1 == y2};
    //const uint clr {c.color()};
    for (int y {0}; y < h; ++y) {
        int xa = x1 + dx13 * (double(y) / h);
        // which part of the triangle
        int xb = (y > dy12 || is_y12) ?  x2 + dx23 * (double(y-dy12) / dy23):
            x1 + dx12 * (double(y) / dy12);
        if (xa > xb) std::swap(xa, xb);
        // fill the triangle
        //const int y_cur {y1 + y};
        draw_horizontal_line(I, c, xa, xb, y1 + y);
        //for (int x {xa}; x <= xb; ++x)
        //    I[x][y_cur] = clr;
    }
}

/*
 * half space algorithm for filling triangle
 * with no optimization compiler options this method is just slightly slower
 * than the standard algorithm. With option 03 turned on, this method becomes
 * faster.
 * This implementation uses a lot of auxilliary variables
 */
void Triangle::fill_hs(RGB_Image &I, const RGB_Color &C) const {
    int y1 {p1_.y()}, y2 {p2_.y()}, y3 {p3_.y()};
    if (y1 == y2 && y1 == y3) return;
    int x1 {p1_.x()}, x2 {p2_.x()}, x3 {p3_.x()}, w = I.width(), h = I.height();
    if (y1 > y2) {std::swap(y1, y2); std::swap(x1, x2); }
    if (y1 > y3) {std::swap(y1, y3); std::swap(x1, x3); }
    if (y2 > y3) {std::swap(y2, y3); std::swap(x2, x3); }
    const int xmax {std::min(w, std::max(std::max(std::max(x1, x2), x3), 0))};
    const int ymax {std::min(h, std::max(y3, 0))};
    int xmin {std::min(w, std::max(std::min(std::min(x1, x2), x3), 0))};
    int ymin {std::min(h, std::max(y1, 0))};
    if (xmax < 0 || ymax < 0 || xmin >= w || ymin >= h) return;
    if ((x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1) < 0)
        std::swap(x1, x3); std::swap(y1, y3);
    static constexpr int q {8}, q1 {q - 1};
    xmin &= ~(q - 1); ymin &= ~(q - 1); // start in corner of qxq block
    const int dx12 {x1 - x2}, dx23 {x2 - x3}, dx31 {x3 - x1};
    const int dy12 {y1 - y2}, dy23 {y2 - y3}, dy31 {y3 - y1};
    int C1 {dy12 * x1 - dx12 * y1}, C2 {dy23 * x2 - dx23 * y2},
        C3 {dy31 * x3 - dx31 * y3};
    if (dy12 < 0 || (dy12 == 0 && dx12 > 0)) ++C1;
    if (dy23 < 0 || (dy23 == 0 && dx23 > 0)) ++C2;
    if (dy31 < 0 || (dy31 == 0 && dx31 > 0)) ++C3;
    for (int y {ymin}; y < ymax; y += q) {
        const int yt0 {y}, yt1 {y + q1}, ytq {y + q};
        const int at0 {C1 + dx12 * yt0}, at1 {C1 + dx12 * yt1};
        const int bt0 {C2 + dx23 * yt0}, bt1 {C2 + dx23 * yt1};
        const int ct0 {C3 + dx31 * yt0}, ct1 {C3 + dx31 * yt1};
        for (int x {xmin}; x < xmax; x += q) {
            const int xt0 {x}, xt1 {x + q1};
            const int ax0 {dy12 * xt0}, ax1 {dy12 * xt1};
            const int a = (at0 - ax0 > 0) | ((at0 - ax1 > 0) << 1) |
                ((at1 - ax0 > 0) << 2) | ((at1 - ax1 > 0) << 3);
            const int bx0 {dy23 * xt0}, bx1 {dy23 * xt1};
            const int b = (bt0 - bx0 > 0) | ((bt0 - bx1 > 0) << 1) |
                ((bt1 - bx0 > 0) << 2) | ((bt1 - bx1 > 0) << 3);
            const int cx0 {dy31 * xt0}, cx1 {dy31 * xt1};
            const int c = (ct0 - cx0 > 0) | ((ct0 - cx1 > 0) << 1) |
                ((ct1 - cx0 > 0) << 2) | ((ct1 - cx1 > 0) << 3);
            if (a == 0x0 || b == 0x0 || c == 0x0) continue;
            const int xtq {x + q};
            if (a == 0xF && b == 0xF && c == 0xF) {
                for (int iy {y}; iy < ytq; ++iy)
                    for (int ix {x}; ix < xtq; ++ix)
                        I(ix, iy) = C;
            } else {
                int Cy1 {at0 - ax0}, Cy2 {bt0 - bx0}, Cy3 {ct0 - cx0};
                for (int iy {y}; iy < ytq; ++iy) {
                    int Cx1 {Cy1}, Cx2 {Cy2}, Cx3 {Cy3};
                    for (int ix {x}; ix < xtq; ++ix) {
                        if (Cx1 > 0 && Cx2 > 0 && Cx3 > 0)
                            I(ix, iy) = C;
                        Cx1 -= dy12; Cx2 -= dy23; Cx3 -= dy31;
                    }
                    Cy1 += dx12; Cy2 += dx23; Cy3 += dx31;
                }
            }
        }
    }
}

/*
 * Calculate barycentric coordinates
 */
//Algebra_lib::Vec<3, double> bary(const Point &p1, const Point &p2,
//        const Point &p3, const Point &p) {
//    const int x1 {p1.x()}, y1 {p1.y()}, x2 {p2.x()}, y2 {p2.y()};
//    const int x3 {p3.x()}, y3 {p3.y()}, x {p.x()}, y {p.y()};
//    const int dx13 {x1 - x3}, dx32 {x3 - x2}, dy13 {y1 - y3}, dy23 {y2 - y3};
//    const double z = dy23 * dx13 + dx32 * dy13;
//    if (std::abs(z) < 0.5) return Algebra_lib::Vec<3, double>{-1, 1, 1};
//    const int dx3 {x - x3}, dy3 {y - y3};
//    const double lam1 {(dy23 * dx3 + dx32 * dy3) / z};
//    const double lam2 {(dx13 * dy3 - dy13 * dx3 ) / z};
//    return Algebra_lib::Vec<3, double>{1 - lam1 - lam2, lam1, lam2};
//}

//void Triangle::fill_bary(RGB_Image &I, const RGB_Color &C) const {
//    const int w = I.width() - 1, h = I.height() - 1;
//    auto xmin = std::max(std::min({p1_.x(), p2_.x(), p3_.x(), w}), 0);
//    auto xmax = std::min(std::max({p1_.x(), p2_.x(), p3_.x(), 0}), w);
//    auto ymin = std::max(std::min({p1_.y(), p2_.y(), p3_.y(), h}), 0);
//    auto ymax = std::min(std::max({p1_.y(), p2_.y(), p3_.y(), 0}), h);
//    const auto clr = C.color();
//    for (auto y = ymin; y <= ymax; ++y)
//        for (auto x = xmin; x <= xmax; ++x) {
//            Algebra_lib::Vec<3, double> vb {bary(p1_, p2_, p3_, Point{x, y})};
//            if (vb.x() >= 0 && vb.y() >= 0 && vb.z() >= 0)
//                I[x][y] = clr;
//        }
//}

