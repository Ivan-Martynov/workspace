#include "PPM_Image.h"
#include "Geometry.h"
#include <iostream>
#include <iterator>
#include <thread>
#include <chrono>

using namespace std;

void draw_square(PPM_Image &img, const PPM_Color &c) {
    const int xc {img.width() >> 1}, yc {img.height() >> 1},
          s {min(xc, yc) >> 2};
    for (int i {-s}; i < s; ++i)
        for (int j {-s}; j < s; ++j)
            img.pixel_color(xc + i, yc + j, c);
}

void test_colour() {
    constexpr int w {150}, h {100};
    PPM_Image img {w, h, PPM_Color{250, 12, 130}};
    draw_square(img, PPM_Color{Color_name::teal});
    write_ppm_image(img, "output.ppm");
}

void test_points() {
    constexpr int w {150}, h {100};
    PPM_Image img {w, h};
    Point p1 {10, 10};
    p1.draw(img);
    Point p2 {w >> 1, h >> 1};
    p2.draw(img);

    write_ppm_image(img, "output.ppm");
}


void test_simple_animation() {
    // open some image viewer which can refresh the image automatically
    using namespace std::chrono;
    // create an image and set color for single point
    constexpr int w {100}, h {100};

    for (int k {0}; k < 3; ++k) {
        PPM_Image img {w, h, PPM_Color(Color_name::khaki)};
        draw_square(img, PPM_Color{Color_name::teal});
        write_ppm_image(img, "output.ppm");

        this_thread::sleep_for(seconds(2));

        img = PPM_Image{w, h};
        draw_square(img, PPM_Color{Color_name::red});
        write_ppm_image(img, "output.ppm");

        this_thread::sleep_for(seconds(2));
    }
}

void test_cases() {
    //test_colour();
    test_points();
    //test_simple_animation();
}

void save_ppm_image(const std::string &fn) {
    std::ofstream ofs {fn, std::ios_base::binary};
    ofs.exceptions(ofs.exceptions() | std::ios_base::badbit);
    const int w {200}, h {300}, num_vals {w * h * 3};
    ofs << "P6\n" << w << ' ' << h << "\n255\n"; // header
    //unsigned int pix;
    //pix = 0xff << 24 | 123 << 16 | 24 << 8 | 0;
    //cout << hex << pix << dec << ' ' << pix << endl;
    std::vector<int> v (num_vals, 0xFF00FF00);
    //std::copy(v.begin(), v.end(), std::ostream_iterator<std::uint8_t>(ofs));
    for (int i {0}; i < num_vals; ++i)
        ofs << v[i];
}

int main() {

    test_cases();
    //save_ppm_image("output.ppm");

    return 0;
}

