#define _POSIX_C_SOURCE 199309L // For clock_gettime and friends.

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "binary_float.h"

static bool almost_equal_floats_ulp(
    const float a, const float b, const float ulp)
{
    const float diff = fabsf(a - b);
    return diff <= FLT_EPSILON * fabsf(a + b) * ulp || diff < FLT_MIN;
}

static bool nearly_equal_floats(
    const float a, const float b, const float epsilon)
{
    if (a == b)
    {
        return true;
    }

    const float abs_diff = fabsf(a - b);
    const float abs_sum = fabsf(a) + fabsf(b);

    if ((a == 0.0f) || (b == 0.0f) || (abs_sum < FLT_MIN))
    {
        return abs_diff < epsilon * FLT_MIN;
    }
    else
    {
        return (abs_sum < FLT_MAX) ? abs_diff < abs_sum * epsilon
                                   : abs_diff < FLT_MAX * epsilon;
    }
}

static size_t test_floats_from_file(
    FILE* const output_stream, const char* const file_path)
{
    FILE* file_ptr = fopen(file_path, "r");

    if (!file_ptr)
    {
        fprintf(stderr, "Unable to open file %s.\n", file_path);
        return 0U;
    }
    else
    {
        size_t count;
        fscanf(file_ptr, "%zu", &count);

        for (size_t i = 0; i < count; ++i)
        {
            float value;
            fscanf(file_ptr, "%f", &value);

            show_bits_float(output_stream, value);
        }

        fclose(file_ptr);

        return count;
    }
}

static int compare_float_files(const char* const first_file_path,
    const char* const second_file_path, const size_t row_count)
{
    int result = -1; // error code.
    FILE* first_file_ptr = fopen(first_file_path, "r");
    if (!first_file_ptr)
    {
        fprintf(stderr, "Unable to open file %s.\n", first_file_path);
        return result;
    }

    FILE* second_file_ptr = fopen(second_file_path, "r");
    if (!second_file_ptr)
    {
        fprintf(stderr, "Unable to open file %s.\n", second_file_path);
        return result;
    }

    result = 0; // All good.
    for (size_t i = 0; i < row_count; ++i)
    {
        char line1[256] = {0};
        char line2[256] = {0};
        if (fgets(line1, 256, first_file_ptr)
            && fgets(line2, 256, second_file_ptr))
        {
            if (strcmp(line1, line2))
            {
                // printf("Lines are not equal => %s & %s\n", line1, line2);
                result = 1; // Found different lines.
            }
        }
    }

    fclose(first_file_ptr);
    fclose(second_file_ptr);

    return result;
}

static double gettime_diff_milliseconds(
    const struct timespec* lhs, const struct timespec* rhs)
{
    return (rhs->tv_sec - lhs->tv_sec) * 1E3
           + (rhs->tv_nsec - lhs->tv_nsec) * 1E-6;
}

#if 0
static double current_time_milliseconds(void)
{
    // struct timespec spec;
    // timespec_get(&spec, TIME_UTC);

    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);

    return spec.tv_sec * 1E3 + spec.tv_nsec * 1E-6;
}
#endif

static void test_from_files(void)
{
#if 0
    const char* const init_path
        = "C:/Users/Ivan/Documents/Misc/Ohjelmointi/eff_c/assignments/"
          "floating.data/floating.";
#else
    const char* const init_path = "floating.data/floating.";
#endif

    for (size_t i = 1; i <= 7; ++i)
    {
        struct timespec start_spec;
        clock_gettime(CLOCK_MONOTONIC, &start_spec);
        // const double d = current_time_milliseconds();

        char file_path[256] = {0};
        sprintf(file_path, "%s%zu%s", init_path, i, ".in");

        const char* const output_file_path = "floating.out";
        FILE* file_ptr = fopen(output_file_path, "w");
        if (!file_ptr)
        {
            fprintf(stderr, "Unable to open file %s.\n", output_file_path);
        }
        else
        {
            const size_t row_count = test_floats_from_file(file_ptr, file_path);

            fclose(file_ptr);

            sprintf(file_path, "%s%zu%s", init_path, i, ".out");

            const int result
                = compare_float_files(file_path, output_file_path, row_count);

            struct timespec end_spec;
            clock_gettime(CLOCK_MONOTONIC, &end_spec);
            const double d = gettime_diff_milliseconds(&start_spec, &end_spec);

            switch (result)
            {
                case 0:
                    printf("%zu: OK! [%g]\n", i, d);
                    break;

                case -1:
                    printf("%zu: Failed file reading [%g].\n", i, d);
                    break;

                case 1:
                    printf("%zu: NOT OK! [%g]\n", i, d);
                    break;

                default:
                    break;
            }
        }
    }
}

static void print_float_trim_zeros(FILE* stream, const float value)
{
    char num_str[256] = {0};

    sprintf(num_str, "%.64f", value);

    size_t i = strlen(num_str);

    while (i-- && (num_str[i] == '0') && (num_str[i] != '.'))
    {
    }
    num_str[(num_str[i] == '.') ? i : i + 1] = '\0';

    if (sign_bit_float(value))
    {
        fprintf(stream, "\tDecimal     %s\n", num_str);
    }
    else
    {
        fprintf(stream, "\tDecimal     +%s\n", num_str);
    }
}

static void print_double_trim_zeros(FILE* stream, const double value)
{
    char num_str[256] = {'\0'};

    sprintf(num_str, "%.128f", value);

    size_t i = strlen(num_str);

    while (i-- && (num_str[i] == '0') && (num_str[i] != '.'))
    {
    }
    num_str[(num_str[i] == '.') ? i : i + 1] = '\0';

    if (sign_bit_double(value))
    {
        fprintf(stream, "\tDecimal     %s\n", num_str);
    }
    else
    {
        fprintf(stream, "\tDecimal     +%s\n", num_str);
    }
}
static void show_float(FILE* const stream, const float value)
{
    // fprintf(stream, "\tDecimal     %.32F\n", value);
    print_float_trim_zeros(stream, value);
    fprintf(stream, "\tScientific  %+.32E\n", value);
    fprintf(stream, "\tHexadecimal %+.8A\n", value);
    fprintf(stream, "\tBits        ");
    show_bits_float(stream, value);
    fprintf(stream, "\tBinary      ");
    print_binary_float(stream, value);
}

static void show_double(FILE* const stream, const double value)
{
    // fprintf(stream, "\tDecimal     %.32F\n", value);
    print_double_trim_zeros(stream, value);
    fprintf(stream, "\tScientific  %+.64E\n", value);
    fprintf(stream, "\tHexadecimal %+.16A\n", value);
    fprintf(stream, "\tBits        ");
    show_bits_double(stream, value);
    fprintf(stream, "\tBinary      ");
    print_binary_double(stream, value);
}

static void test_floats(void)
{
    show_bits_float(stdout, +1.0f);
    show_bits_float(stdout, -1.0f);
    show_bits_float(stdout, +0.0f);
    show_bits_float(stdout, -0.0f);
    show_bits_float(stdout, INFINITY);
    show_bits_float(stdout, -INFINITY);
    show_bits_float(stdout, NAN);
    show_bits_float(stdout, -NAN);
    show_bits_float(stdout, 1.0f / 3.0f);
    show_bits_float(stdout, FLT_MIN * 0.5f);
    show_bits_float(
        stdout, -0.00000000000000000000000000000000000001175494500000f);
    unsigned int i = 0x1;
    float* f = (float* const)(&i);
    show_float(stdout, *f);
    i = 0x7FFFFF;
    show_bits_float(stdout, *f);
}

static bool report_float_pair_test(const float a, const float b,
    const float eps,
    bool compare_function(const float, const float, const float),
    const bool result)
{
    // if (almost_equal_floats_v1(a, b, 2) != result)
    if (compare_function(a, b, eps) != result)
    {
        const char* const expected_bool = result ? "true" : "false";
        const char* const received_bool = result ? "false" : "true";

        fprintf(stderr,
            "Failed float comparison:\n\tFirst = %.53f\n\tSecond = "
            "%.53f\n\tExpected %s, received %s\n",
            a, b, expected_bool, received_bool);

        return false;
    }
    else
    {
        return true;
    }
}

static void test_pair_floats(const float a, const float b, const float eps,
    bool compare_function(const float, const float, const float),
    const bool result)
{
    report_float_pair_test(a, b, eps, compare_function, result);
    report_float_pair_test(b, a, eps, compare_function, result);
    report_float_pair_test(-a, -b, eps, compare_function, result);
    report_float_pair_test(-b, -a, eps, compare_function, result);
}

static void float_examples(void)
{
    printf("Smallest subnormal:\n");
    show_float(stdout, float_from_bits(0x1));

    printf("Largest subnormal:\n");
    show_float(stdout, float_from_bits(0x7FFFFF));

    printf("Smallest normal:\n");
    show_float(stdout, float_from_bits(1 << 23));

    printf("Largest normal:\n");
    show_float(stdout, float_from_bits(0x7F7FFFFF));

    const float positive_zero = float_from_bits(0x0);
    printf("Positive zero:\n");
    show_float(stdout, positive_zero);

    const float negative_zero = float_from_bits(1 << 31);
    printf("Negative zero:\n");
    show_float(stdout, negative_zero);

    printf("Positive infinity:\n");
    show_float(stdout, INFINITY);

    printf("Negative infinity:\n");
    show_float(stdout, -INFINITY);

    printf("NaN from sqrtf(-1.0f):\n");
    show_float(stdout, sqrtf(-1.0f));

    printf("NaN from -0.0 / 0.0:\n");
    show_float(stdout, negative_zero / positive_zero);

    float nan_inf_inf = INFINITY - INFINITY;
    printf("NaN from inf - inf:\n");
    show_float(stdout, nan_inf_inf);

    ++(*(uint32_t*)&nan_inf_inf);
    printf("Next from NaN from inf - inf:\n");
    show_float(stdout, nan_inf_inf);
}

static void double_examples(void)
{
    printf("Smallest subnormal:\n");
    show_double(stdout, double_from_bits(0x1UL));

    printf("Largest subnormal:\n");
    show_double(stdout, double_from_bits(0xFFFFFFFFFFFFFUL));

    printf("Smallest normal:\n");
    show_double(stdout, double_from_bits(1UL << 52UL));

    printf("Largest normal:\n");
    show_double(stdout, double_from_bits(0x7FFFFFFFFFFFFFFFUL));

    const float positive_zero = double_from_bits(0x0);
    printf("Positive zero:\n");
    show_double(stdout, positive_zero);

    const float negative_zero = double_from_bits(1UL << 63UL);
    printf("Negative zero:\n");
    show_double(stdout, negative_zero);

    printf("Positive infinity:\n");
    show_double(stdout, HUGE_VAL);

    printf("Negative infinity:\n");
    show_double(stdout, -HUGE_VAL);

    printf("NaN from sqrt(-1.0):\n");
    show_double(stdout, sqrt(-1.0));

    printf("NaN from -0.0 / 0.0:\n");
    show_double(stdout, negative_zero / positive_zero);

    double nan_inf_inf = HUGE_VAL - HUGE_VAL;
    printf("NaN from HUGE_VAL - HUGE_VAL:\n");
    show_double(stdout, nan_inf_inf);

    ++(*(uint64_t*)&nan_inf_inf);
    printf("Next from NaN from HUGE_VAL - HUGE_VAL:\n");
    show_double(stdout, nan_inf_inf);
}

void test_float_comparison(void)
{
    const float eps = 0.00001f;
    test_pair_floats(1000000.0f, 1000001.0f, eps, nearly_equal_floats, true);
    test_pair_floats(-1000.0f, -1001.0f, eps, nearly_equal_floats, false);

    test_pair_floats(1.000001f, 1.000002f, eps, nearly_equal_floats, true);
    test_pair_floats(1.001f, 1.002f, eps, nearly_equal_floats, false);

    test_pair_floats(
        0.000000001000001f, 0.000000001000002f, eps, nearly_equal_floats, true);
    test_pair_floats(0.000000000001002f, 0.000000000001001f, eps,
        nearly_equal_floats, false);

    test_pair_floats(0.3f, 0.30000003f, eps, nearly_equal_floats, true);

    test_pair_floats(0.0f, 0.0f, eps, nearly_equal_floats, true);
    test_pair_floats(-0.0f, 0.0f, eps, nearly_equal_floats, true);

    test_pair_floats(0.0f, 0.0000001f, eps, nearly_equal_floats, false);

    test_pair_floats(INFINITY, INFINITY, eps, nearly_equal_floats, true);
    test_pair_floats(INFINITY, -INFINITY, eps, nearly_equal_floats, false);

    test_pair_floats(NAN, INFINITY, eps, nearly_equal_floats, false);

    test_pair_floats(NAN, 0.0, eps, nearly_equal_floats, false);

    const float smallest_subnormal = float_from_bits(0x1);
    const float largest_subnormal = float_from_bits(0x7FFFFF);

    const float smallest_normal = float_from_bits(1 << 23);
    const float largest_normal = float_from_bits(0x7F7FFFFF);

    const float positive_zero = float_from_bits(0x0);
    const float negative_zero = float_from_bits(1 << 31);

    test_pair_floats(smallest_subnormal, largest_subnormal, 2,
        almost_equal_floats_ulp, true);
    test_pair_floats(
        smallest_subnormal, positive_zero, 2, almost_equal_floats_ulp, true);
    test_pair_floats(
        smallest_subnormal, negative_zero, 2, almost_equal_floats_ulp, true);
    test_pair_floats(
        smallest_normal, positive_zero, 1E-6f, nearly_equal_floats, false);
    test_pair_floats(
        smallest_normal, negative_zero, 1E-6f, nearly_equal_floats, false);

    const float nan_sqrt_minus_one = sqrtf(-1.0f);
    const float nan_zero_over_zero = positive_zero / negative_zero;
    const float nan_inf_minus_inf = INFINITY - INFINITY;

    test_pair_floats(largest_normal, nan_sqrt_minus_one, FLT_EPSILON,
        nearly_equal_floats, false);
    test_pair_floats(
        largest_normal, INFINITY, FLT_EPSILON, nearly_equal_floats, false);
    test_pair_floats(
        INFINITY, nan_zero_over_zero, FLT_EPSILON, nearly_equal_floats, false);
    test_pair_floats(nan_inf_minus_inf, nan_inf_minus_inf, FLT_EPSILON,
        nearly_equal_floats, false);

    test_pair_floats(2.0f, 1.9999995f, eps, nearly_equal_floats, true);

    test_pair_floats(FLT_MAX, FLT_MAX, eps, nearly_equal_floats, true);

    test_pair_floats(FLT_MIN, FLT_MIN, 2, nearly_equal_floats, true);
    test_pair_floats(FLT_MIN, -FLT_MIN, 2, nearly_equal_floats, true);
    test_pair_floats(FLT_MIN, positive_zero, 2, nearly_equal_floats, true);
    test_pair_floats(positive_zero, -FLT_MIN, 2, nearly_equal_floats, true);
    test_pair_floats(FLT_MIN, negative_zero, 2, nearly_equal_floats, true);
    test_pair_floats(negative_zero, -FLT_MIN, 2, nearly_equal_floats, true);

    test_pair_floats(FLT_MIN, 0.000000001f, eps, nearly_equal_floats, false);
    test_pair_floats(-FLT_MIN, 0.000000001f, eps, nearly_equal_floats, false);
}

int main()
{
    test_floats();
    test_from_files();

    float_examples();
    test_float_comparison();

    double_examples();

    return EXIT_SUCCESS;
}
