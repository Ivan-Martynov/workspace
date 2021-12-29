#include "PPM_Image.h"
#include "Geometry.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <future>

void print_color(const PPM_lib::RGB_Color& c) {
    std::cout << "red: " << +c.red() << ", green: " << +c.green() <<
        ", blue: " << +c.blue() << '\n';
}

auto num_of_threads(const size_t num_elems, const size_t min_per_thread = 25) {
    return std::min(std::max(std::thread::hardware_concurrency(), 0u),
            static_cast<unsigned int>((num_elems + min_per_thread - 1) /
                min_per_thread));
}

// measuring time for a function
auto time_func = [](size_t n, auto&& f, auto&&... pars) {
    using namespace std::chrono;
    auto t = high_resolution_clock::now();
    // call function n times
    for (size_t i {0}; i < n; ++i)
        std::forward<decltype(f)>(f)(std::forward<decltype(pars)>(pars)...);
    return high_resolution_clock::now() - t;
};

void test_color() {
    using namespace PPM_lib;
    auto c1 = RGB_Color();
    std::cout << "c1, ";
    print_color(c1);
    c1.red() = 125;
    c1.green() = 68;
    c1.blue() = 12;
    std::cout << "c1, ";
    print_color(c1);
    auto c2 = RGB_Color("0xFF12342");
    std::cout << "c2, ";
    print_color(c2);
}

// random int class
class Rand_int {
public:
    Rand_int(int low_val, int high_val): p_ {low_val, high_val} { }
    int operator()() const noexcept { return r(); }
private:
    std::uniform_int_distribution<>::param_type p_;
    std::function<int()> r = [p = p_]() {
        std::uniform_int_distribution<> dist{p};
        std::random_device rdev;
        std::default_random_engine deng(rdev());
        return dist(deng);
    };
};

auto random_point(const int max1, const int max2) {
    Rand_int r1 {0, max1}, r2 {0, max2};
    return Geometry_lib::Point{r1(), r2()};
}

void add_rand_points(PPM_lib::RGB_Image& I, const PPM_lib::RGB_Color& clr,
        const Rand_int& r1, const Rand_int& r2, const int n) {
    for (auto i = 0; i < n; ++i)
        I(r1(), r2()) = clr;
}

void test_points() {
    using namespace PPM_lib;
    using namespace Geometry_lib;
    auto I = RGB_Image {600, 400, RGB_Color{255, 255, 255}};
    const auto max1 = I.width() - 1, max2 = I.height() - 1;
    const Rand_int r1 {0, max1}, r2 {0, max2};
    constexpr auto num_points = 100;
    constexpr auto point_color = RGB_Color {0, 0, 255};
    auto num_threads = num_of_threads(num_points);
    auto block_size  = num_points / num_threads;
    std::vector<std::thread> threads(num_threads - 1);
    for (size_t i {0}; i < num_threads - 1; ++i)
        threads[i] = std::thread{[&I, r1, r2, point_color, block_size]()
            {
                for (size_t j {0}; j < block_size; ++j)
                    I(r1(), r2()) = point_color;
            }};
    for (size_t j {0}; j < num_points - block_size * (num_threads - 1); ++j)
        I(r1(), r2()) = point_color;
    for (auto& t: threads)
        t.join();

    Triangle tri1 {10, 10, 120, 10, 180, 200};
    tri1.draw(I, RGB_Color{255, 128, 0});

    I.write_to("test.ppm");
}

void test_image_reading() {
    using namespace PPM_lib;
    const RGB_Image I {"../imgs/baboon.ppm"};
    //const RGB_Image I {600, 400};
    print_color(I(210, 245));
    I.write_to("test.ppm");
}

int sum_points(const std::vector<int>& v, const int n) {
    int res {0};
    for (int i {0}; i < n; ++i)
        res += v[i];
    return res;
}

void test_sum_points() {
    const auto num_elems = 1E6;
    std::vector<int> v(num_elems);
    Rand_int r {0, 10};
    for (auto& e: v)
        e = r();
    //const auto num_threads = std::thread::hardware_concurrency();
    //std::cout << vec_sum(v, num_threads) << '\n';
    auto f1 = std::async(std::launch::async, sum_points, v, 50);
    auto f2 = std::async(std::launch::async, sum_points, v, 50);
    std::cout << f1.get() + f2.get() << '\n';
}

int main() {

    //auto t = time_func(1, test_color);
    auto t = time_func(1, test_points);
    //auto t = time_func(1, test_image_reading);

    std::cout << std::chrono::duration<double>(t).count() << '\n';

    //std::cout << std::thread::hardware_concurrency() <<
    //    " concurrent threads are supported\n";

    //test_sum_points();

    return 0;
}

