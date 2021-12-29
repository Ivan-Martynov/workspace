/*
 * Very simple class for writing RGB PPM images
 * Uses 3 bytes (24 bits) per pixel: standard for PPM RGB images
 */

#ifndef _PPM_IMAGE_H_
#define _PPM_IMAGE_H_

#include <fstream>
#include <vector>

using uchar_t = unsigned char; // just to type less

static constexpr int bpp {3}; // bytes (not bits) per pixel

enum class Color_name {
    red, green, blue, black, white, cyan, magenta, yellow, orange, teal, aqua,
    brown, chocolate, coral, darkblue, darkcyan, darkgrey, darkgreen, darkkhaki,
    darkmagenta, darkorange, darkred, darkviolet, firebrick, gold, indigo, lime,
    khaki, maroon, pink, purple, skyblue, snow, violet
};

class PPM_Color {
public:
    PPM_Color();
    PPM_Color(const uchar_t);
    PPM_Color(const uchar_t, const uchar_t, const uchar_t);
    PPM_Color(const Color_name);
    PPM_Color(const PPM_Color&);
    PPM_Color &operator=(const PPM_Color&);

    ~PPM_Color() = default;

    uchar_t red() const { return r_; }
    uchar_t green() const { return g_; }
    uchar_t blue() const { return b_; }

private:
    uchar_t r_;
    uchar_t g_;
    uchar_t b_;
};

class PPM_Image {
public:
    PPM_Image();
    PPM_Image(const int, const int, const PPM_Color&);
    PPM_Image(const int, const int, const Color_name cn = Color_name::black);
    PPM_Image(const PPM_Image&);
    PPM_Image &operator=(const PPM_Image&);

    ~PPM_Image() = default;

    friend std::ostream &operator<<(std::ostream&, const PPM_Image&);

    int width() const { return width_; }
    int height() const { return height_; }
    std::vector<uchar_t> values() const { return vals_; }

    void pixel_color(int, int, const PPM_Color&);

private:
    int width_;
    int height_;
    std::vector<uchar_t> vals_;
};

void write_ppm_image(const PPM_Image&, const std::string&);

#endif

