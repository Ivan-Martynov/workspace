#include "PPM_Image.h"
#include <iostream>
#include <memory>
#include <exception>

using namespace PPM_lib;

// use image values from a char array and cast them to the proper type
RGB_Color::RGB_Color(const std::unique_ptr<char[]>& v, int idx):
    r_{static_cast<unsigned char>(v[idx])},
    g_{static_cast<unsigned char>(v[idx + 1])},
    b_{static_cast<unsigned char>(v[idx + 2])} {
    }

// helper function: check if input is six value hex
bool is_valid_hex(const std::string &s) {
    if (s.length() < 6) return false;
    int idx {0};
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        idx = 2;
    if (s.length() - idx != 6)
        return false;
    for (size_t i = idx; i < s.length(); ++i)
        if (!isxdigit(s[i]))
            return false;
    return true;
}

/*
 * Helper function: converting hex string to unsigned int
 * Checking the first part of the string, if the string is long, the remaining
 * part is ignored
 */
unsigned int hex_str_to_num(const std::string& s) {
    // number of digit characters used for conversion
    const size_t num_chars {6};
    auto n = s.length();
    if (n < num_chars)
        return 0;
    size_t idx {0};
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        idx = 2;
    if (n - idx < num_chars)
        return 0;
    for (auto i = idx; i < n; ++i)
        if (!isxdigit(s[i]))
            return 0;
    return static_cast<unsigned int>(stoul(s.substr(idx, 6), nullptr, 16));
}

// construct color from a hex string
RGB_Color::RGB_Color(const std::string& s): RGB_Color{hex_str_to_num(s)} {
}
//RGB_Color::RGB_Color(const std::string& s): RGB_Color{is_valid_hex(s) ?
//    static_cast<unsigned int>(stoul(s, nullptr, 16)) : 0} {
//    }
//RGB_Color::RGB_Color(const std::string& s)
//    try: RGB_Color{static_cast<unsigned int>(stoul(s, nullptr, 16))} {
//    } catch (const std::invalid_argument&) {
//        RGB_Color{};
//    } catch (const std::out_of_range&) {
//        RGB_Color{};
//    } catch (...) {
//        RGB_Color{};
//    }

// RGB color output operator
std::ostream& operator<<(std::ostream& os, const RGB_Color& c) {
    return os << c.red() << c.green() << c.blue();
}

RGB_Image::RGB_Image(const int w, const int h, const RGB_Color& c):
    w_{w}, h_{h}, I_{std::vector<value_type>(w_ * h_, c)} { }

// helper function: skip commment lines in the header of the ppm image
void skip_comment(std::istream &is) {
    char c;
    // in a ppm image file a comment line begins with character #
    while (is >> c && c == '#')
        is.ignore(256, '\n'); // skipping comment lines
    // put the last read character into the buffer to make it accessible later
    is.unget();
}

// constructor calling a function to read image data from a file
RGB_Image::RGB_Image(const std::string& fn): w_{}, h_{}, I_{} {
    using namespace std;

    // read the file and use exceptions settings
    ifstream ifs {fn, ios_base::binary};
    if (!ifs)
        throw runtime_error("cannot open file " + fn);
    ifs.exceptions(ifs.exceptions() | ios_base::badbit);

    string header; // read file header
    if (!(ifs >> header) || header != "P6")
        throw runtime_error("cannot read input file");
    skip_comment(ifs); // skipping comment lines

    // image width, height and maximum intensity (which is not used here)
    int intensity_max;
    ifs >> w_ >> h_ >> intensity_max;
    skip_comment(ifs); // skipping comment lines

    // parameters for vector size and number of values in the image file:
    // the value is multiplied by three because a color pixel is represented by
    // red, green and blue intensity values
    const auto vec_size = w_ * h_, num_values = vec_size * 3;

    // read image values
    auto v = std::make_unique<char[]>(num_values);
    ifs.read(v.get(), num_values);

    // reserve space for the vector (since the size is known and to avoid
    // allocation during pushing back
    I_.reserve(vec_size);
    // a single pixel value is reprented by RGB values, which are located
    // consecutively and put into the data vector combined as an unsigned int
    for (int i {0}; i < vec_size; ++i)
        I_.emplace_back(v, i * 3);
        //I_.push_back(RGB_Color{v, i * 3});
}

//std::ostream& operator<<(std::ostream& os, const RGB_Image& I) {
//    os << "P6\n" << I.w_ << ' ' << I.h_ << "\n255\n"; // header
//    for (const auto& c: I.I_)
//        os << c;
//    return os;
//}

// write a RGB_Image data into a ppm file
void RGB_Image::write_to(const std::string &fn) const {
    // open file for reading with exceptions settings
    std::ofstream ofs {fn, std::ios_base::binary};
    ofs.exceptions(ofs.exceptions() | std::ios_base::badbit);

    ofs << "P6\n" << w_ << ' ' << h_ << "\n255\n"; // header
    for (const auto& c: I_)
        ofs << c;
    std::cout << "The result is saved to file: " << fn << '\n';
}

