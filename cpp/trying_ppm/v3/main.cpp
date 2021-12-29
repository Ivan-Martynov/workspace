#include "PPM_lib.h"
#include <iostream>
#include <bitset>

void write_bw() {
    constexpr int w {200}, h {100}, n {w * h};
    using namespace std;
    using uchar = unsigned char;
    vector<uchar> vals(n);
    for (auto &x: vals) x = 1;

    std::ofstream ofs {"bw.ppm", std::ios_base::binary};
    ofs.exceptions(ofs.exceptions() | std::ios_base::badbit);
    ofs << "P4\n" << w << ' ' << h << "\n"; // header

    constexpr int m = (n >> 3) << 3;
    for (int i {0}; i < m; i += 8) {
        uchar bw_val {0};
        for (int j {0}; j < 8; ++j) {
            bw_val += (vals[i + j] == 0) << (j);
            //cout << +bw_val << '\n';
        }
        ofs << bw_val;
        //ofs << (0xFF ^ bw_val);
    }
}

void test_gray() {
    using namespace std;
    using namespace PPM_lib;

    constexpr int w {237}, h {143};
    GS_Image I (w, h);
    for (int y {0}; y < h >> 1; ++y) {
        for (int x {0}; x < w >> 1; ++x)
            I[x][y] = 127;
        for (int x {w >> 1}; x < w; ++x)
            I[x][y] = 190;
    }
    for (int y {h >> 1}; y < h; ++y) {
        for (int x {0}; x < w >> 1; ++x)
            I[x][y] = 64;
        for (int x {w >> 1}; x < w; ++x)
            I[x][y] = 255;
    }

    I.write_to("gray.ppm");

    GS_Image I2 {"../building.ppm"};

    I2.write_to("building_gray.ppm");
}

void test_rgb() {
    using namespace PPM_lib;

    RGB_Image I {"../building_gray.ppm"};
    I.write_to("rgb.ppm");

    GS_Image GI {I};
    GI.write_to("gray.ppm");
}

void test_bitset() {
    using namespace std;
    bitset<8> b1 {42};
    cout << b1 << '\t' << b1.to_ulong() << '\n';
    const int n = 10000;
    cout << ((n >> 3) << 3) << '\n';
}

void test_gray_color() {
    using namespace PPM_lib;
    constexpr PPM_lib::GS_Color gc1;
    constexpr GS_Color gc2 {182};
    std::cout << +gc1.color() << '\n';
    std::cout << +gc2.color() << '\n';
}

int main() {

    //test_bitset();
    //write_bw();
    //test_gray();
    test_rgb();
    //test_gray_color();

    return 0;
}

