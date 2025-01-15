#include "structures/pnm_format/pnm_image_base.h"
#include "structures/geometric/point2d.h"
#include "tools/random_number.h"
#include "tools/RandomUniformNumber.h"

#include <filesystem>
#include <algorithm>
#include <thread>

namespace
{

[[maybe_unused]]
void test_pnm_images(const std::filesystem::path& image_path)
{
    auto process_subdir_lambda {
        [&image_path](const char* const subdir_name, const char* const suffix,
            std::ios_base::openmode open_mode)
        {
            for (const auto& entry :
                std::filesystem::directory_iterator(image_path / subdir_name))
            {
                const auto path {entry.path()};

                std::string_view comment {
                    "Comment about the width of this image. Another dummy "
                    "comment which, perhaps, will exceed the line length "
                    "limit."};
                Marvin::PNM_Format::make_image_ptr(path.string().c_str())
                    ->write_to((path.parent_path().parent_path()
                                   / (path.stem().string() + suffix
                                       + path.extension().string()))
                                   .string()
                                   .c_str(),
                        open_mode, comment);
            }
        }};

    process_subdir_lambda("raw", "_to_plain", {});
    process_subdir_lambda("plain", "_to_raw", std::ios_base::binary);
}

void fractal(double left, double top, double x_side, double y_side,
    Marvin::PNM_Format::PPMImage& image)
{
    static constexpr int max_count {30};

    const auto width = image.width();
    const auto height = image.height();

    const auto x_scale {x_side / width};
    const auto y_scale {y_side / height};

    using size_type = Marvin::PNM_Format::PPMImage::size_type;

    for (size_type row {0}; row < height; ++row)
    {
        for (size_type col {0}; col < width; ++col)
        {
            const auto cx = col * x_scale + left;
            const auto cy = row * y_scale + top;

            double zx {0.0};
            double zy {0.0};
            size_type count {0};

            while ((zx * zx + zy * zy < 4) && (count++ < max_count))
            {
                const auto temp_x {zx * zx - zy * zy + cx};
                zy = 2 * zx * zy + cy;
                zx = temp_x;
            }

#if 0
            count = static_cast<size_type>(
                count * (image.max_value() / static_cast<double>(max_count)));

            // image[row, col] = {count, count, count};
            // image[row, col] = {count, count, count};
            image[row, col] = {count, 0, 0};
#else
            image[row, col] = Marvin::RGBColor::from_intmax(
                static_cast<int>(count * (65535.0 / max_count)));
#endif
        }
    }
}

[[maybe_unused]]
/**
 * @brief Create Cantor's line segments on the image on both sides of the
 * horizontal center line, which is used a the starting line for the Cantor's
 * set.
 *
 * @remark Not using an iteration count here, because for most images there are
 * not too many steps before a line degenerates.
 *
 * @param[in] start Start of the segment.
 * @param[in] end End of the segment.
 * @param[in] image Image to write to.
 * @param[in] row Row of the image to place the current segments on.
 */
void add_Cantor(
    int start, int end, Marvin::PNM_Format::PBMImage& image, int row)
{
    static constexpr int stripe_width {5};

    const auto step {(end - start) / 3};
    if ((step <= 0) || (row <= 0) || (row >= image.height()))
    {
        return;
    }

    const auto next {start + (step << 1)};
    for (int i {std::max(0, row - stripe_width + 1)};
         i < std::min(image.height(), row + stripe_width - 1); ++i)
    {
        for (int col {start}; col < start + step; ++col)
        {
            image[i, col] = 1;
        }

        for (int col {next}; col < end; ++col)
        {
            image[i, col] = 1;
        }
    }

    constexpr auto row_step {stripe_width << 1};
    add_Cantor(start, start + step, image, row - row_step);
    add_Cantor(start, start + step, image, row + row_step);
    add_Cantor(next, end, image, row - row_step);
    add_Cantor(next, end, image, row + row_step);
}

[[maybe_unused]]
void make_Cantor_fractal(
    Marvin::PNM_Format::PBMImage&& image, const char* const file_path)
{
    add_Cantor(0, image.width(), image, image.height() >> 1);
    image.write_to(file_path);
}

[[maybe_unused]]
void make_Cantor_fractal()
{
    make_Cantor_fractal({27 * 27, 600}, "images/fractals/cantor_01.pbm");
    make_Cantor_fractal({81, 27}, "images/fractals/cantor_02.pbm");
}

[[maybe_unused]]
void make_Sierpinski_triangle(int)
{
    //Marvin::Geometric::Point2d p {};
//    int x1 {0}; int y1 {0};
//    int x2 {128}; int y2 {0};
//    // 110 * 100 < sqrt(128 * 128 - 64 * 64) < 111 * 111
//    int x3 {64}; int y3 {111};
//    Marvin::PNM_Format::PPMImage image{129, 129};

}

/**
 * @brief Divide an interval into desired number of blocks/chunks.
 *
 * @param[in] first One end of the interval.
 * @param[in] second Another end of the interval.
 * @param[in] count Number of blocks/chunks to create.
 * @return std::vector<int> Vector containing interval points, each consecutive
 * pair representing a block/chunk.
 */
std::vector<int> linspace(int first, int second, int count)
{
    constexpr double epsilon {1E-9};
    std::vector<int> result(count + 1);
    const auto step {static_cast<double>(second - first) / count};
    result[0] = first;
    for (int i {1}; i < count; ++i)
    {
        // Add a small number to compensate for possible floating point
        // imprecision.
        result[i] = static_cast<int>(first + step * i + epsilon);
    }
    result[count] = second;

    return result;
}

void show_linspace(int first, int second, int count)
{
    std::cout << "Linspace for (" << first << ", " << second << ", " << count
              << ")\n";
    std::ranges::for_each(
        linspace(first, second, count), [](int x) { std::cout << x << " "; });
    std::cout << "\n";
}

[[maybe_unused]]
void test_linspace()
{
    show_linspace(10, 40, 4);
    show_linspace(10, 0, 2);
    show_linspace(0, 10, 3);
    show_linspace(0, 10, 7);
    show_linspace(10, 0, 19);
    show_linspace(17, 100, 17);
}

void fill_in_threads(
    Marvin::PNM_Format::PPMImage& image, const Marvin::RGBColor& color)
{
    using namespace Marvin::PNM_Format;
    using size_type = PPMImage::size_type;

    const auto w {image.width()};
    const auto h {image.height()};
    const auto n {w * h};
    auto thread_count {std::min(
        n, static_cast<size_type>(std::thread::hardware_concurrency()))};
//        thread_count = 7;
    std::vector<std::thread> threads {};
    threads.reserve(thread_count);
    //const auto step {static_cast<double>(n) / thread_count};
    const auto chunks {linspace(0, n, thread_count)};
    //size_type start {0};
    for (size_type i {0}; i < thread_count; ++i)
    {
        // const auto start {i * step};
        // const auto end {i != thread_count - 1 ? start + step : n};
        //const auto end {
        //    i != thread_count - 1
        //        ? static_cast<size_type>(step * (i + 1) + 1E-6)
        //        : n};

        const auto start {chunks[i]};
        const auto end {chunks[i + 1]};
        std::cout << "Start = " << start << "; end = " << end << "\n";

        threads.push_back(std::thread {[start, end, &image, &color]()
            {
                for (auto j {start}; j < end; ++j)
                {
                    // No need for locking as every pixel index is unique.
                    image[j] = color;
                }
            }});
    }
    std::ranges::for_each(threads, [](auto& t) { t.join(); });
}

void random_points_in_threads(Marvin::PNM_Format::PPMImage& image,
    const Marvin::PNM_Format::PPMImage::size_type point_count,
    const Marvin::RGBColor& point_color)
{
    using namespace Marvin::PNM_Format;
    using size_type = PPMImage::size_type;

    auto thread_count {std::min(image.width() * image.height(),
        static_cast<size_type>(std::thread::hardware_concurrency()))};

    std::mutex mutex {};
    auto add_random_point_lambda {
        [&image, thread_count, &mutex, point_count, &point_color]()
        {
            const auto n {image.width() * image.height()};
            for (auto j {0}; j < point_count / thread_count; ++j)
            {
                std::lock_guard<std::mutex> guard {mutex};
                image[Marvin::RandomNumber::generate(0, n - 1)] = point_color;
            }
        }};

    std::vector<std::thread> threads {};
    threads.reserve(thread_count);
    for (size_type i {0}; i < thread_count; ++i)
    {
        threads.push_back(std::thread {add_random_point_lambda});
    }

    std::ranges::for_each(threads, [](auto& t) { t.join(); });
}

[[maybe_unused]]
void create_grid_image()
{
    using namespace Marvin::PNM_Format;
    PPMImage image {800, 600};
    image.fill(Marvin::RGBColor{128, 128, 128});
    const int grid_x_count {20};
    const int grid_y_count {20};
    const int cell_size_x {image.width() / grid_x_count};
    const int cell_size_y {image.height() / grid_y_count};

    auto fill_cell_lambda {[cell_size_x, cell_size_y, &image](
                               int x, int y, const Marvin::RGBColor& color) {
        for (int cell_x {1}; cell_x < cell_size_x - 1; ++cell_x) {
            for (int cell_y {1}; cell_y < cell_size_y - 1; ++cell_y) {
                image[y * cell_size_y + cell_y, x * cell_size_x + cell_x]
                    = color;
            }
        }
    }};

    for (int col {0}; col < grid_x_count; ++col) {
        for (int row {0}; row < grid_y_count; ++row) {
            fill_cell_lambda(row, col, Marvin::RGBColor {255, 255, 255});
        }
    }

    fill_cell_lambda(5, 7, Marvin::RGBColor {0, 0, 0});

    image.write_to("images/grid.ppm");
}

[[maybe_unused]]
void create_rgb_image()
{
    using namespace Marvin::PNM_Format;

#if 1 // Test new class.
    PNMImageTemplated<Marvin::BlackWhiteColor> p {10, 8};
    // const auto t {p.plain_type()};
    // std::cout << std::to_underlying(t) << "\n";
    //std::cout << p.values().size() << "\n";
#endif

    constexpr int width {4};
    constexpr int height {2};

    PPMImage image_01 {width, height};
    for (int row {0}; row < height >> 1; ++row)
    {
        for (int col {0}; col < width >> 1; ++col)
        {
            image_01[row, col] = Marvin::RGBColor {image_01.max_value(), 0, 0};
        }

        for (int col {width >> 1}; col < width; ++col)
        {
            image_01[row, col] = Marvin::RGBColor {
                image_01.max_value(), image_01.max_value(), 0};
        }
    }

    for (int row {height >> 1}; row < height; ++row)
    {
        for (int col {0}; col < width >> 1; ++col)
        {
            image_01[row, col] = Marvin::RGBColor {0, image_01.max_value(), 0};
        }

        for (int col {width >> 1}; col < width; ++col)
        {
            image_01[row, col] = Marvin::RGBColor {0, 0, image_01.max_value()};
        }
    }

    // image_01.scale(400, 200);
    image_01.scale(127.8);
    image_01.write_to("images/ppm/test_01.ppm");

    PPMImage image_02 {800, 600, 32};
    auto left = -1.75;
    auto top = -0.25;
    auto xside = 0.25;
    auto yside = 0.45;
    fractal(left, top, xside, yside, image_02);
    image_02.resize(401, 273);
    // image_02.scale(500, 250);

    // auto m {0};
    // for (const auto& color : image_02.data())
    //{
    //     m = std::max({m, color.red(), color.green(), color.blue()});
    // }
    // std::cout << "Max = " << m << "\n";

    image_02.write_to("images/ppm/test_02.ppm");

#if 0
    PPMImage image_03 {5, 6};
#else
    PPMImage image_03 {200, 200};
#endif

    fill_in_threads(image_03, {128, 0, 0});

//    image_03.fill({128, 0, 0});
    image_03.fill_row(
        image_03.height() - (image_03.height() / 3), {0, 255, 255});
    image_03.fill_column(image_03.width() >> 2, {0, 0, 255});

    random_points_in_threads(image_03, 100, {255, 255, 255});

    image_03.write_to("images/ppm/test_03.ppm");
}

[[maybe_unused]]
void test_random()
{
    constexpr int sample_count {10};
    Marvin::RandomUniformNumber gen_01 {1, 5};
    for (int i {0}; i < sample_count; ++i) {
        std::cout << gen_01.generate() << ' ';
    }
    std::cout << '\n';

    Marvin::RandomUniformNumber<long long int> gen_02 {25};
    for (int i {0}; i < sample_count; ++i) {
        std::cout << gen_02.generate() << ' ';
    }
    std::cout << '\n';

    Marvin::RandomUniformNumber<short> gen_03 {};
    for (int i {0}; i < sample_count; ++i) {
        std::cout << gen_03.generate() << ' ';
    }
    std::cout << '\n';

    Marvin::RandomUniformNumber<float> gen_04 {};
    for (int i {0}; i < sample_count; ++i) {
        std::cout << gen_04.generate() << ' ';
    }
    std::cout << '\n';

    Marvin::RandomUniformNumber<double> gen_05 {-2.3, 3.8};
    for (int i {0}; i < sample_count; ++i) {
        std::cout << gen_05.generate() << ' ';
    }
    std::cout << '\n';

    std::vector<int> v {};
    v.reserve(sample_count);
    std::generate_n(std::back_insert_iterator(v), sample_count,
        [&gen_01]() { return gen_01.generate(); });
    for (auto x : v)
    {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

} // namespace

int main()
{
    // test_pnm_images("images/pbm/");
    // test_pnm_images("images/pgm/");
    // test_pnm_images("images/ppm/");

    //create_rgb_image();
    // make_Cantor_fractal();

    //test_linspace();
    //create_grid_image();
    test_random();

    return 0;
}
