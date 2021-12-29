#include <iostream>
#include "CImg.h" // image library

void test_image_opening() {
    using namespace cimg_library;
    //CImg<unsigned char> I ("../images/lena.png");
    CImg<unsigned char> I (600, 600, 3);
    for (int i {100}; i < 210; ++i)
        for (int j {100}; j < 210; ++j) {
            I(i, j, 0) = 255;
            I(i, j, 1) = 255;
            I(i, j, 2) = 255;
        }
    I(100, 500, 0) = 255;
    I(100, 500, 1) = 255;
    I(100, 500, 2) = 255;
    std::cout << +I(100, 500, 0) << '\n';
    std::cout << +I(100, 500, 1) << '\n';
    std::cout << +I(100, 500, 2) << '\n';
    CImgDisplay main_display(I);
    while (!main_display.is_closed()) {
        main_display.wait();
    }
}

int main() {

    test_image_opening();

    return 0;
}

