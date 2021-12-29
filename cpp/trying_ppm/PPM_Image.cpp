#include <iostream>
#include "PPM_Image.h"

PPM_Color::PPM_Color(): r_{0}, g_{0}, b_{0} {
}

PPM_Color::PPM_Color(const uchar_t c): r_{c}, g_{c}, b_{c} { }

PPM_Color::PPM_Color(const uchar_t r, const uchar_t g, const uchar_t b):
    r_{r}, g_{g}, b_{b} {
    }

PPM_Color::PPM_Color(const Color_name c): r_{}, g_{}, b_{} {
    switch (c) {
        case Color_name::red:
            r_ = 255; g_ = 0; b_ = 0;
            break;
        case Color_name::green:
            r_ = 0; g_ = 255; b_ = 0;
            break;
        case Color_name::blue:
            r_ = 0; g_ = 0; b_ = 255;
            break;
        case Color_name::black:
            r_ = 0; g_ = 0; b_ = 0;
            break;
        case Color_name::white:
            r_ = 255; g_ = 255; b_ = 255;
            break;
        case Color_name::cyan:
            r_ = 0; g_ = 255; b_ = 255;
            break;
        case Color_name::magenta:
            r_ = 255; g_ = 0; b_ = 255;
            break;
        case Color_name::yellow:
            r_ = 255; g_ = 255; b_ = 0;
            break;
        case Color_name::orange:
            r_ = 255; g_ = 165; b_ = 0;
            break;
        case Color_name::teal:
            r_ = 0; g_ = 128; b_ = 128;
            break;
        case Color_name::aqua:
            r_ = 0; g_ = 255; b_ = 255;
            break;
        case Color_name::brown:
            r_ = 165; g_ = 42; b_ = 42;
            break;
        case Color_name::chocolate:
            r_ = 210; g_ = 105; b_ = 30;
            break;
        case Color_name::coral:
            r_ = 255; g_ = 127; b_ = 80;
            break;
        case Color_name::darkblue:
            r_ = 0; g_ = 0; b_ = 139;
            break;
        case Color_name::darkcyan:
            r_ = 0; g_ = 139; b_ = 139;
            break;
        case Color_name::darkgrey:
            r_ = 169; g_ = 169; b_ = 169;
            break;
        case Color_name::darkgreen:
            r_ = 0; g_ = 100; b_ = 0;
            break;
        case Color_name::darkkhaki:
            r_ = 189; g_ = 183; b_ = 107;
            break;
        case Color_name::darkmagenta:
            r_ = 139; g_ = 0; b_ = 139;
            break;
        case Color_name::darkorange:
            r_ = 255; g_ = 140; b_ = 0;
            break;
        case Color_name::darkred:
            r_ = 139; g_ = 0; b_ = 0;
            break;
        case Color_name::darkviolet:
            r_ = 148; g_ = 0; b_ = 211;
            break;
        case Color_name::firebrick:
            r_ = 178; g_ = 34; b_ = 34;
            break;
        case Color_name::gold:
            r_ = 255; g_ = 215; b_ = 0;
            break;
        case Color_name::indigo:
            r_ = 75; g_ = 0; b_ = 130;
            break;
        case Color_name::lime:
            r_ = 0; g_ = 255; b_ = 0;
            break;
        case Color_name::khaki:
            r_ = 240; g_ = 230; b_ = 140;
            break;
        case Color_name::maroon:
            r_ = 128; g_ = 0; b_ = 0;
            break;
        case Color_name::pink:
            r_ = 255; g_ = 192; b_ = 203;
            break;
        case Color_name::purple:
            r_ = 128; g_ = 0; b_ = 128;
            break;
        case Color_name::skyblue:
            r_ = 135; g_ = 206; b_ = 235;
            break;
        case Color_name::snow:
            r_ = 255; g_ = 250; b_ = 250;
            break;
        case Color_name::violet:
            r_ = 238; g_ = 130; b_ = 238;
            break;
    }
}

PPM_Color::PPM_Color(const PPM_Color &o): r_{o.red()}, g_{o.green()},
    b_{o.blue()} {
}

PPM_Color &PPM_Color::operator=(const PPM_Color &o) {
    if (this != &o) {
        r_ = o.red(); g_ = o.green(); b_ = o.blue();
    }
    return *this;
}

PPM_Image::PPM_Image(): width_{0}, height_{0}, vals_{} {
}

PPM_Image::PPM_Image(const int w, const int h, const PPM_Color& c):
    width_{std::max(0, w)}, height_{std::max(0, h)},
    vals_{std::vector<uchar_t>(width_ * height_ * bpp)} {
        if (w <= 0 || h <= 0) {
            std::cerr << "warning: negative dimensions, empty image created\n";
        } else {
            const uchar_t r {c.red()}, g {c.green()}, b {c.blue()};
            if (r == g && g == b)
                for (auto &x: vals_)
                    x = r;
            else
                for (size_t i = 0; i < vals_.size(); i += bpp) {
                    vals_[i] = r;
                    vals_[i + 1] = g;
                    vals_[i + 2] = b;
                }
        }
    }

PPM_Image::PPM_Image(const int w, const int h, const Color_name cn):
    width_{std::max(0, w)}, height_{std::max(0, h)},
    vals_{std::vector<uchar_t>(width_ * height_ * bpp)} {
        if (w <= 0 || h <= 0) {
            std::cerr << "warning: negative dimensions, empty image created\n";
        } else {
            const PPM_Color c {cn};
            const uchar_t r {c.red()}, g {c.green()}, b {c.blue()};
            if (r == g && g == b)
                for (auto &x: vals_)
                    x = r;
            else
                for (size_t i = 0; i < vals_.size(); i += bpp) {
                    vals_[i] = r;
                    vals_[i + 1] = g;
                    vals_[i + 2] = b;
                }
        }
    }

PPM_Image::PPM_Image(const PPM_Image &o): width_{o.width()},
    height_{o.height()}, vals_{o.values()} {
    }

PPM_Image &PPM_Image::operator=(const PPM_Image &o) {
    if (this != &o) {
        width_ = o.width();
        height_ = o.height();
        vals_ = o.values();
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const PPM_Image &img) {
    const int w {img.width()}, h {img.height()}, num_vals {w * h * bpp};
    os << "P6\n" << w << ' ' << h << "\n255\n"; // header
    std::vector<uchar_t> v {img.values()};
    for (int i {0}; i < num_vals; i += bpp)
        for (int j {0}; j < bpp; ++j)
            os << v[i + j];
    return os;
}

void write_ppm_image(const PPM_Image &img, const std::string &fn) {
    std::ofstream ofs {fn, std::ios_base::binary};
    ofs.exceptions(ofs.exceptions() | std::ios_base::badbit);
    ofs << img;
}

void PPM_Image::pixel_color(int x, int y, const PPM_Color &c) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "error: coordinates are beyond the image\n";
        return;
    } else {
        const int idx {(x + y * width_) * bpp};
        vals_[idx] = c.red();
        vals_[idx + 1] = c.green();
        vals_[idx + 2] = c.blue();
    }
}

