#define _POSIX_C_SOURCE 199309L // For clock_gettime and friends.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "binary_float.h"

static bool nearly_equal_floats(
    const float a, const float b, const float epsilon)
{
    if (a == b)
    {
        return true;
    }

    const float a_abs = fabsf(a);
    const float b_abs = fabsf(b);
    const float diff = fabsf(a - b);

    if ((a == 0.0f) || (b == 0.0f) || (a_abs + b_abs < FLT_MIN))
    {
        return diff < epsilon * FLT_MIN;
    }
    else
    {
        return diff / fmin(a_abs + b_abs, FLT_MAX) < epsilon;
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

            show_float_bits(output_stream, value);
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
                //printf("Lines are not equal => %s & %s\n", line1, line2);
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
        //const double d = current_time_milliseconds();

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

static void show_float(FILE* stream, const float value)
{
    fprintf(stream, "\tDecimal     %.53G\n", value);
    fprintf(stream, "\tScientific  %.16E\n", value);
    fprintf(stream, "\tHexadecimal %.16A\n", value);
    fprintf(stream, "\tBits        ");
    show_float_bits(stream, value);
}

static void test_floats(void)
{
    show_float_bits(stdout, +1.0f);
    show_float_bits(stdout, -1.0f);
    show_float_bits(stdout, +0.0f);
    show_float_bits(stdout, -0.0f);
    show_float_bits(stdout, INFINITY);
    show_float_bits(stdout, -INFINITY);
    show_float_bits(stdout, NAN);
    show_float_bits(stdout, -NAN);
    show_float_bits(stdout, 1.0f / 3.0f);
    show_float_bits(stdout, FLT_MIN * 0.5f);
    show_float_bits(
        stdout, -0.00000000000000000000000000000000000001175494500000f);
    unsigned int i = 0x1;
    float* f = (float* const)(&i);
    show_float(stdout, *f);
    //show_float_bits(stdout, *f);
    i = 0x7FFFFF;
    show_float_bits(stdout, *f);
}

static void float_examples(void)
{
    unsigned int i = 0x1;
    float* const f = (float* const)(&i);

    printf("Smallest subnormal:\n");
    show_float(stdout, *f);

    i = 0x7FFFFF;

    printf("Largest subnormal:\n");
    show_float(stdout, *f);

    i = 1 << 23;

    printf("Smallest normal:\n");
    show_float(stdout, *f);

    i = 0x7F7FFFFF;

    printf("Largest normal:\n");
    show_float(stdout, *f);
}

void test_pair_floats(
    const float a, const float b, const float eps, const bool result)
{
    assert(nearly_equal_floats(a, b, eps) == result);
    assert(nearly_equal_floats(b, a, eps) == result);
    assert(nearly_equal_floats(-a, -b, eps) == result);
    assert(nearly_equal_floats(-b, -a, eps) == result);
}

void test_float_comparison(void)
{
    const float eps = 0.00001f;
    test_pair_floats(1000000.0f, 1000001.0f, eps, true);
    test_pair_floats(-1000.0f, -1001.0f, eps, false);

    test_pair_floats(1.000001f, 1.000002f, eps, true);
    test_pair_floats(1.001f, 1.002f, eps, false);

    test_pair_floats(0.000000001000001f, 0.000000001000002f, eps, true);
    test_pair_floats(0.000000000001002f, 0.000000000001001f, eps, false);

    test_pair_floats(0.3f, 0.30000003f, eps, true);

    test_pair_floats(0.0f, 0.0f, eps, true);

    test_pair_floats(0.0f, 0.0000001f, eps, false);
}

int main()
{
    test_floats();
    test_from_files();

    //show_float(stdout, 1.0f);
    float_examples();
    test_float_comparison();

    return EXIT_SUCCESS;
}
