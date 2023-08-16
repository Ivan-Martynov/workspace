#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "sorting_algos.h"

int compare_doubles(const void* const arg1, const void* const arg2)
{
    return *(const double*)arg1 - *(const double*)arg2;
}

int compare_strings(const void* const arg1, const void* const arg2)
{
    return strcmp(
        *(const char* const* const)arg1, *(const char* const* const)arg2);
}

void print_int(const void* const arg)
{
    printf("%d ", *(const int* const)arg);
}

void print_double(const void* const arg)
{
    printf("%f ", *(const double* const)arg);
}

void print_string(const void* const arg)
{
    printf("%s ", *(const char** const)arg);
}

void random_array(const size_t n)
{
    srand(time(NULL));

    double array[n];

    for (size_t i = 0; i < n; ++i)
    {
        array[i] = rand();
    }

    clock_t start = clock();
    merge_sort_d(array, 0, n - 1);
    start = clock() - start;

    printf("Is random array[%zu] merge sorted? %d; time: %f\n", n,
        is_sorted_general(array, n, sizeof(double), compare_doubles),
        (double)start / CLOCKS_PER_SEC);
    // quick_sort_d(array, 0, count - 1);
    // for (size_t i = 0; i < n; ++i)
    //{
    //	printf("%f ", array[i]);
    //}
    // puts("");
}

int main()
{
    printf("C version: %ld\n", __STDC_VERSION__);
    //fflush(stdout);

    const size_t random_array_size = 1E5;
    random_array(random_array_size);

    clock_t start = clock();
    time_t t1;
    time(&t1);
    printf("Current time: %f\n", (double)t1);
    // printf("Current nsec: %ld\n", ts.tv_nsec);

    double array[] = {38.0, 27.0, 43.0, 3.0, 9.0, 82.0, 10.0, 19.0};
    // double array[] = { 38.0, 27.0, };
    size_t count = sizeof(array) / sizeof(array[0]);

    print_array_general(array, count, sizeof(double), print_double);
    merge_sort_d(array, 0, count - 1);
    quick_sort_d(array, 0, count - 1);
    // bubble_sort(array, count);
    printf("%d\n",
        is_sorted_general(array, count, sizeof(double), compare_doubles));
    bubble_sort_general(array, count, sizeof(double), compare_doubles);
    print_array_general(array, count, sizeof(double), print_double);
    printf("%d\n",
        is_sorted_general(array, count, sizeof(double), compare_doubles));

    const char* str1 = "How do you do!";
    const char* str2 = "Hello World!";
    const size_t string_count = 2;
    const char* strings[string_count];

    strings[0] = str1;
    strings[1] = str2;

    for (size_t i = 0; i < 1; ++i)
    {
        print_array_general(
            strings, string_count, sizeof(const char*), print_string);
        printf("%d\n", is_sorted_general(strings, string_count,
                           sizeof(const char*), compare_strings));
        bubble_sort_general(
            strings, string_count, sizeof(const char*), compare_strings);
        print_array_general(
            strings, string_count, sizeof(const char*), print_string);
        printf("%d\n", is_sorted_general(strings, string_count,
                           sizeof(const char*), compare_strings));
    }

    clock_t end = clock();
    double t = ((double)(end - start)) / CLOCKS_PER_SEC;

    time_t t2;
    time(&t2);

    printf("Testing time: clock => %f; time => %f\n", t, (double)(t2 - t1));

    return 0;
}