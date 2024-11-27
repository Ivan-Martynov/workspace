#include "structures/pnm_format/pnm_image_base.h"
#include "structures/colors/blackwhite_color.h"
#include "structures/colors/grayscale_color.h"
#include "structures/colors/rgb_color.h"

#include <filesystem>
#include <algorithm>

[[maybe_unused]]
static void test_reading_pbm()
{
    using namespace Marvin::PNM_Format;

#if 1
    const auto test_comment {
        "Comment about a black and white image representing letter J. Trying "
        "to make it longer than the default line length limit."};
#else
    const auto test_comment {
        "ad;fka;dfajkd;"
        "sfajoiewapefajpeaejpoiewjpqojewpqiwjqpwejpqfiejpqqpeiKdk:"
        "EIIPE qpep qije qpjpqwei jqewjpq eejfuqwpeqweijq pwe"};
#endif
    PPMImage rgb_plain {};
    PGMImage gs_plain {};
    // PBMImage j_plain {};
    PBMImage j_plain {};
    j_plain.read_from("images/pbm/raw/j.pbm");
    j_plain.write_to("images/pbm/j_to_raw.pbm", HeaderComments {test_comment});
    j_plain.write_to("images/pbm/j_to_raw.pbm", std::ios_base::app);

    HeaderComments comments {test_comment};
    // comments.main_comment = test_comment;
    comments.width_comment
        = "Comment about the width of this image. Another dummy comment which, "
          "perhaps, will exceed the line length limit.";
    comments.height_comment
        = "Height of this image has been informed about. Another dummy comment "
          "which, perhaps, will exceed the line length limit.";
    comments.max_value_comment
        = "This image has fixed max value. Another dummy comment "
          "which, perhaps, will not appear in the file...";

    PBMImage j_raw {};
    j_plain.read_from("images/pbm/j_to_raw.pbm");
    j_plain.write_to("images/pbm/j_raw_to_plain.pbm",
        std::ios_base::openmode {}, HeaderComments {test_comment});
}

static void test_single_rgb(const Marvin::RGBColor& color)
{
    const auto value {static_cast<std::intmax_t>(color)};
    std::cout << color << " => " << value << " => "
              << Marvin::RGBColor::from_intmax(value) << "\n";
}

[[maybe_unused]]
static void test_rgb_color()
{
    test_single_rgb(Marvin::RGBColor {});
    test_single_rgb(Marvin::RGBColor {255, 0, 255});
    test_single_rgb(Marvin::RGBColor {255, 255, 255});
}

[[maybe_unused]]
static void test_blackwhite_color()
{
    Marvin::BlackWhiteColor color_01 {};
    std::cout << color_01 << "\n";

    Marvin::BlackWhiteColor color_02 {true};
    std::cout << color_02 << "\n";
}

[[maybe_unused]]
static void test_grayscale_color()
{
    Marvin::GrayScaleColor color_01 {};
    std::cout << std::boolalpha << color_01 << "\n";

    Marvin::GrayScaleColor color_02 {128};
    std::cout << color_02 << "\n";
}

[[maybe_unused]]
static void test_pnm_images(const std::filesystem::path& image_path)
{
    auto process_subdir_lambda {
        [&image_path](const char* const subdir_name, const char* const suffix,
            std::ios_base::openmode open_mode)
        {
            for (const auto& entry :
                std::filesystem::directory_iterator(image_path / subdir_name))
            {
                const auto path {entry.path()};

                Marvin::PNM_Format::HeaderComments comments {
                    "main comment about file at " + path.string()};
                comments.width_comment
                    = "Comment about the width of this image. Another dummy "
                      "comment which, "
                      "perhaps, will exceed the line length limit.";
                comments.height_comment
                    = "Height of this image has been informed about. Another "
                      "dummy comment "
                      "which, perhaps, will exceed the line length limit.";
                comments.max_value_comment
                    = "This image has fixed max value. Another dummy comment "
                      "which, perhaps, will not appear in the file... but who "
                      "knows";

                Marvin::PNM_Format::make_image_ptr(path.string().c_str())
                    ->write_to((path.parent_path().parent_path()
                                   / (path.stem().string() + suffix
                                       + path.extension().string()))
                                   .string()
                                   .c_str(),
                        open_mode, comments);
            }
        }};

    process_subdir_lambda("raw", "_to_plain", {});
    process_subdir_lambda("plain", "_to_raw", std::ios_base::binary);
}

static void fractal(double left, double top, double x_side, double y_side,
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
static void add_Cantor(
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
static void make_Cantor_fractal(
    Marvin::PNM_Format::PBMImage&& image, const char* const file_path)
// Marvin::PNM_Format::PBMImage&& image, const char* const file_path)
{
    add_Cantor(0, image.width(), image, image.height() >> 1);
    image.write_to(file_path);
}

[[maybe_unused]]
static void make_Cantor_fractal()
{
    make_Cantor_fractal({27 * 27, 600}, "images/fractals/cantor_01.pbm");
    make_Cantor_fractal({81, 27}, "images/fractals/cantor_02.pbm");
}

[[maybe_unused]]
static void create_rgb_image()
{
    using namespace Marvin::PNM_Format;

#if 1 // Test new class.
    PNMImageTemplated<Marvin::BlackWhiteColor> p {10, 8};
    // const auto t {p.plain_type()};
    // std::cout << std::to_underlying(t) << "\n";
    std::cout << p.values().size() << "\n";
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
}

int main()
{
    // test_rgb_color();
    // test_grayscale_color();
    // test_blackwhite_color();

    test_pnm_images("images/pbm/");
    test_pnm_images("images/pgm/");
    test_pnm_images("images/ppm/");

    test_reading_pbm();

    //    create_rgb_image();
    // make_Cantor_fractal();

    return 0;
}
