#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include <fstream>
#include <memory>
#include <vector>

namespace PPM_lib { // general namespace for this small library

class RGB_Color {
public:
    // conventional alias to type a bit less
    using value_type = unsigned char;

    /*
     * Constructors
     */
    // default: all values set to zero
    constexpr explicit RGB_Color() noexcept: r_{0}, g_{0}, b_{0} { }

    // constructor with three values for RGB components
    constexpr explicit RGB_Color(const value_type, const value_type,
            const value_type) noexcept;

    // constructor taking an unsigned int representing color
    constexpr explicit RGB_Color(const unsigned int) noexcept;

    // reading values from a char array at given index
    explicit RGB_Color(const std::unique_ptr<char[]>&, int);

    // constructor taking a string (char array)
    explicit RGB_Color(const std::string&);

    RGB_Color(const RGB_Color&) = default;
    RGB_Color& operator=(const RGB_Color&) = default;
    RGB_Color(RGB_Color&&) = default;
    RGB_Color& operator=(RGB_Color&&) = default;
    ~RGB_Color() = default;

    constexpr value_type  red()   const noexcept { return r_; }
    constexpr value_type& red()   noexcept { return r_; }
    constexpr value_type  green() const noexcept { return g_; }
    constexpr value_type& green() noexcept { return g_; }
    constexpr value_type  blue()  const noexcept { return b_; }
    constexpr value_type& blue()  noexcept { return b_; }

    constexpr static value_type red(const unsigned int c) noexcept {
        return static_cast<value_type>(c >> 16 & 0xFF);
    }
    constexpr static value_type green(const unsigned int c) noexcept {
        return static_cast<value_type>(c >> 8 & 0xFF);
    }
    constexpr static value_type blue(const unsigned int c) noexcept {
        return static_cast<value_type>(c & 0xFF);
    }
private:
    value_type r_, g_, b_;
};

// constructor with three RGB values
constexpr RGB_Color::RGB_Color(const value_type r, const value_type g,
        const value_type b) noexcept: r_{r}, g_{g}, b_{b} { }

// constructor with color value represented as unsigned int
constexpr RGB_Color::RGB_Color(const unsigned int c) noexcept: r_{red(c)},
          g_{green(c)}, b_{blue(c)} {
          }

// constructor taking a string (char array)
//constexpr RGB_Color::RGB_Color(const auto s) {
//}

class RGB_Image {
public:
    using value_type = RGB_Color;

    // image of size w * h
    explicit RGB_Image(const int, const int, const value_type& = value_type{});
    // read a ppm image from a file
    explicit RGB_Image(const std::string&);

    RGB_Image(const RGB_Image&) = default;
    RGB_Image& operator=(const RGB_Image&) = default;
    RGB_Image(RGB_Image&&) = default;
    RGB_Image& operator=(RGB_Image&&) = default;
    ~RGB_Image() = default;

    int width() const noexcept { return w_; }
    int height() const noexcept { return h_; }

    // calculate value at column i and row j
    const value_type operator()(int i, int j) const noexcept {
        return I_[w_ * j + i];
    }
    value_type& operator()(int i, int j) noexcept { return I_[w_ * j + i]; }

    void write_to(const std::string&) const;
private:
    //friend std::ostream& operator<<(std::ostream&, const RGB_Image&);
    int w_, h_;
    std::vector<value_type> I_;
};

} // end of namespace PPM_lib

#endif /* PPM_IMAGE_H */

