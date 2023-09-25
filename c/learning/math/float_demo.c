#define _POSIX_C_SOURCE 199309L // For clock_gettime and friends.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include "binary_float.h"

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
                printf("Lines are not equal => %s & %s\n", line1, line2);
                result = 1; // Found different lines.
            }
        }
    }

    fclose(first_file_ptr);
    fclose(second_file_ptr);

    return result;
}

static double current_time_milliseconds(void)
{
    // struct timespec spec;
    // timespec_get(&spec, TIME_UTC);

    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);

    return spec.tv_sec * 1E3 + spec.tv_nsec * 1E-6;
}

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
        const double d = current_time_milliseconds();

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

            switch (compare_float_files(file_path, output_file_path, row_count))
            {
                case 0:
                    printf(
                        "%zu: OK! [%g]\n", i, current_time_milliseconds() - d);
                    break;

                case -1:
                    printf("%zu: Failed file reading [%g].\n", i,
                        current_time_milliseconds() - d);
                    break;

                case 1:
                    printf("%zu: NOT OK! [%g]\n", i,
                        current_time_milliseconds() - d);
                    break;

                default:
                    break;
            }
        }
    }
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
    show_float_bits(stdout, *f);
    i = 0x7FFFFF;
    show_float_bits(stdout, *f);
}

int main()
{
    test_floats();
    test_from_files();

    return EXIT_SUCCESS;
}
