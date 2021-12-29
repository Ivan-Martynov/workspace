#include "PPM_Image.h"
#include "Geometry.h"
#include <iostream>

void test_PPM_Image() {
    constexpr int w {600}, h {400};
    const std::string fn {"output.ppm"};
    PPM_Image I {w, h, Color_name::blue};

    I.set_bgcolor({"0x000000"});
    I.set_color(w >> 1, h >> 1, Color_name::red);
    for (int i {0}; i < w >> 1; ++i)
        for (int j {0}; j < h >> 1; ++j)
            I[i][j] = PPM_Color{"0x23AAC2"}.color();
    I.set_bgcolor(Color_name::red);
    //I[w >> 1][h >> 1] = 0x00FF25;
    //I[100][120] = PPM_Color{"0x23AAC2"}.color();
    //I.set_color(200, 100, Color_name::red);
    //I.set_color(200, 100);
    //PPM_Color c = I.bgcolor();
    PPM_Color clr1 = I.color(10, 10);
    std::cout << clr1 << '\n';
    clr1 = {"0xFF00FF"};
    //I.set_color(300, 100, c);
    //I.set_color(30, 100, Color_name::white);

    std::cout << I.width() << '\n';

    I.write_to(fn);
    PPM_Image J {"building.ppm"};
    J.write_to("test.ppm");
}

void test_lines() {
    constexpr int w {600}, h {400};
    const std::string fn {"output.ppm"};
    PPM_Image I {w, h, Color_name::khaki};

    const Point p1 {w >> 1, h >> 1}, p2 {50, 50};
    const Line s1 {p1, p2};
    s1.draw(I, {234, 12, 142});
    Line{p1, Point {50, 300}}.draw(I, Color_name::blue);
    I.set_bgcolor(Color_name::white);

    I.write_to(fn);
}

void test_triangles() {
    constexpr int w {600}, h {400};
    const std::string fn {"output.ppm"};
    PPM_Image I {w, h, Color_name::khaki};

    const Point p1 {10, 100}, p2 {350, 50}, p3 {150, 300};
    Triangle t1 {p1, p2, p3};
    std::cout << "Perimeter: " << t1.length() << ", area: " << t1.area() << '\n';
    t1.fill(I, Color_name::green);
    t1.draw(I, Color_name::blue);

    I.write_to(fn);

}

int main() {

    test_PPM_Image();
    //test_lines();
    //test_triangles();

    return 0;
}

