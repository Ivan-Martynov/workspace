#include <iostream>
#include <future>
#include "Point2D.h"
#include "Point3D.h"
#include "Polygon.h"
#include "Log_disc.h"
#include "Log_surface.h"

#include "Plot_OpenGL.h"

void test_point() {
    using namespace log_measurement;
    using Point = Point2D<double>;

    constexpr Point p1;
    std::cout << "p1 = " << p1 << '\n';

    constexpr auto p2 = p1 + Point{2, 3} + Point{4, 5} + Point{-3, 2} + p1;
    std::cout << "p2 = " << p2 << '\n';

    Point p3 = p2 + 5.0;
    p3 += Point{4.8, -2.3} / 2 + -9.5;
    std::cout << "p3 = " << p3 << '\n';

    constexpr Point p4 = 2 - Point{2.3, -1.8} * 4 + p1;
    std::cout << "p4 = " << p4 << '\n';
    std::cout << p2.squared_dist_to(p4) << '\n';
    std::cout << p2.dist_to(p4) << '\n';

    Point p5 = p4;
    std::cout << p5 << '\n';
    //p5.swap(p3);
    swap(p3, p5);
    std::cout << p5 << '\n';
    //swap(p1, p4);
}

void test_point3() {
    using namespace log_measurement;
    using Point = Point3D<double>;
    constexpr Point p1 {2, 5, 8}, p2 {3, 1, 4};
    std::cout << p1 << ' ' << p2 << ' ' << p1.cross(p2) << '\n';
}

void test_polygon() {
    using namespace log_measurement;
    using Point = Point2D<double>;
    using Polygon = Polygon<double>;

    Polygon pgn1;
    pgn1.push_back(Point{3.5, 8.2});
    pgn1.emplace_back(4, 7);
    std::cout << pgn1.size() << '\n';

    Polygon pgn2 {std::vector<Point> {Point{1, 1}, Point{4, 1}, Point{4, 5}}};
    std::cout << pgn2.perimeter() << ' ' << pgn2.signed_area() << '\n';
    swap(pgn1, pgn2);
    std::cout << pgn2.perimeter() << ' ' << pgn2.signed_area() << '\n';
}

void test_folder() {
    using namespace log_measurement;

    auto files = directory_files("../vec");
    for (const auto& f: files)
        std::cout << f << '\n';
}

void test_log_disc() {
    using namespace log_measurement;
    using Point = Point2D<double>;

    Log_disc d1 {std::vector<Point> {Point{1, 1}, Point{4, 1}, Point{4, 5}}, 2};
    for (const auto& p: d1)
        std::cout << p << '\n';
    std::cout << d1.height() << '\n';
}

void test_log_surface() {
    using namespace log_measurement;

    const std::string files_path{"../../log_sorting/logPointsTxt/surfaces4"};
    auto files = directory_files(files_path);

    Log_surface log_data1 {files.front()};
    //for (const auto& d: log_data1)
    //    std::cout << d.height() << '\n';
    std::cout << log_data1.volume() * 1E-9 << '\n';
}

void plot_opengl_figure(Plot2D_OpenGL figure) {
    figure.plot();
}

int main() {

    //test_point3();
    //test_polygon();
    //test_folder();
    //test_log_disc();

    const auto t = log_measurement::time_func(1, test_log_surface);
    //const auto t = log_measurement::time_func(1, test_point);

    std::cout << "Execution time: " <<
        std::chrono::duration<double>(t).count() << " seconds\n";

    //auto t1 = std::thread(test_log_surface);

    Plot2D_OpenGL figure1 {600, 400};
    figure1.plot();

    //Plot2D_OpenGL figure2 {600, 400};
    ////figure2.plot();

    //auto t1 = std::thread(plot_opengl_figure, figure1);
    //auto t2 = std::thread(plot_opengl_figure, figure2);
    //auto f1 = std::async(std::launch::async, plot_opengl_figure, figure1);
    //auto f2 = std::async(std::launch::async, plot_opengl_figure, figure2);
    //f1.get();
    //f2.get();

    //t1.join();
    //t2.join();

    return 0;
}

