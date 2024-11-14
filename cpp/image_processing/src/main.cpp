#include "structures/pnm_format/pnm_image_base.h"
#include "structures/pnm_format/pbm_image.h"
#include "structures/pnm_format/pgm_image.h"
#include "structures/pnm_format/ppm_image.h"
#include "structures/colors/blackwhite_color.h"
#include "structures/colors/grayscale_color.h"
#include "structures/colors/rgb_color.h"

[[maybe_unused]]
static void test_pnm_format()
{
    using namespace Marvin::PNM_Format;
    PNMImageBase image;
    //std::ifstream file_stream {"images/feep_plain.pgm", std::ios_base::in};
    std::ifstream file_stream {"images/feep.pgm", std::ios_base::in};
    image.read_header(file_stream);
}

[[maybe_unused]]
static void test_reading_pbm()
{
    using namespace Marvin::PNM_Format;
#if 1
    PBMImage image {};
    image.read_from("images/pbm/feep.pbm");
    image.write_to(
        "images/pbm/feep_raw_to_binary.pbm", PNMImageBase::Mode::raw);

    PBMImage image_plain {};
    image_plain.read_from("images/pbm/feep_raw_to_plain.pbm");
    image_plain.write_to(
        "images/pbm/feep_plain_to_binary.pbm", PNMImageBase::Mode::raw);
    image_plain.write_to(
        "images/pbm/feep_plain_to_plain.pbm", PNMImageBase::Mode::plain);

    PBMImage image_raw {};
    image_raw.read_from("images/pbm/feep_plain_to_plain.pbm");
    image_raw.write_to(
        "images/pbm/feep_raw_to_text.pbm", PNMImageBase::Mode::plain);
#endif

    PBMImage j_plain {};
    j_plain.read_from("images/pbm/j.pbm");
    j_plain.write_to("images/pbm/j_to_raw.pbm", PNMImageBase::Mode::raw);

    PBMImage j_raw {};
    j_plain.read_from("images/pbm/j_to_raw.pbm");
    j_plain.write_to(
        "images/pbm/j_raw_to_plain.pbm", PNMImageBase::Mode::plain);
}

[[maybe_unused]]
static void test_pgm_image()
{
    using namespace Marvin::PNM_Format;

    PGM_Image image_raw {PGM_Image::read_from_file(
        "images/feep.pgm", std::ios_base::binary | std::ios_base::in)};
    PGM_Image image_plain {
        PGM_Image::read_from_file("images/feep_plain.pgm", std::ios_base::in)};

    image_raw.write_to("images/feep_raw_to_binary.pgm", std::ios::binary | std::ios::out);
    image_plain.write_to("images/feep_plain_to_binary.pgm", std::ios::binary | std::ios::out);

    PGM_Image image_binary {PGM_Image::read_from_file(
        "images/feep_plain_to_binary.pgm", std::ios::binary | std::ios::in)};
    image_binary.write_to("images/feep_binary_to_plain.pgm", std::ios::out);

    //PGM_Image image2 {PGM_Image::read_from_file(
    //    "images/feep_binary.pgm", std::ios_base::binary | std::ios_base::in)};
}

[[maybe_unused]]
static void test_ppm_image()
{
    using namespace Marvin::PNM_Format;
    PPMImage image_raw {};
    image_raw.read_from("images/ppm/pbmlib.ppm");
    image_raw.write_to(
        "images/ppm/pbmlib_to_plain.ppm", PNMImageBase::Mode::plain);
    image_raw.write_to(
        "images/ppm/pbmlib_to_raw.ppm", PNMImageBase::Mode::raw);
    PPMImage image_plain {};
    image_plain.read_from("images/ppm/pbmlib.ascii.ppm");
    image_plain.write_to(
        "images/ppm/pbmlib.ascii_to_raw.ppm", PNMImageBase::Mode::raw);
}


static void test_single_rgb(const Marvin::RGBColor& color)
{
    const auto value {static_cast<std::intmax_t>(color)};
    std::cout << color << " => " << value << " => "
              << Marvin::RGBColor::from_intmax(value) << "\n";
}

static void test_rgb_color()
{
    test_single_rgb(Marvin::RGBColor {});
    test_single_rgb(Marvin::RGBColor {255, 0, 255});
    test_single_rgb(Marvin::RGBColor {255, 255, 255});
}

static void test_blackwhite_color()
{
    Marvin::BlackWhiteColor color_01 {};
    std::cout << color_01 << "\n";

    Marvin::BlackWhiteColor color_02 {true};
    std::cout << color_02 << "\n";
}

static void test_grayscale_color()
{
    Marvin::GrayScaleColor color_01 {};
    std::cout << std::boolalpha << color_01 << "\n";

    Marvin::GrayScaleColor color_02 {128};
    std::cout << color_02 << "\n";
}

int main()
{
    // test_pgm_image();
    // test_pnm_format();
    // test_reading_pbm();
    test_rgb_color();
    test_grayscale_color();
    test_blackwhite_color();

    test_ppm_image();

    return 0;
}
