#include <stdio.h>
#include <stdbool.h>

#include "sorting_algos.h"

/*
**  Utils
*/

/**
 * @brief Swap two block of memory given equal size.
 *
 * @param arg1 Pointer to the first block.
 * @param arg2 Pointer to the second block.
 * @param size Size of blocks.
 *
 * @version 0.1
 *
 * @author Ivan Martynov
 *
 * @date 2023-08-21
 *
 * @copyright Copyright (c) 2023
 */
void swap_voids(
    unsigned char* const arg1, unsigned char* const arg2, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        const unsigned char tmp = arg1[i];
        arg1[i] = arg2[i];
        arg2[i] = tmp;
    }
}

void swap_doubles(double* a, double* b)
{
#if 1
    swap_voids((void*)a, (void*)b, sizeof(double));
#else
    double temp = *a;
    *a = *b;
    *b = temp;
#endif
}

void print_array_general(const void* ptr, const size_t count, const size_t size,
    void (*print_element)(const void* const))
{
    for (size_t i = 0; i < count; ++i)
    {
        print_element(ptr);
        ptr = (unsigned char* const)ptr + size;
    }
    printf("\n");
}

void print_array_d(const double arr[static 1], const size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        printf("%f ", arr[i]);
    }
    printf("\n");
}

bool is_sorted_d(const double arr[static 1], const size_t count)
{
    for (size_t i = 1; i < count; i++)
    {
        if (arr[i - 1] > arr[i])
        {
            return false;
        }
    }

    return true;
}

bool is_sorted_general(void* ptr, const size_t count, const size_t size,
    int compare_func(const void*, const void*))
{
    for (size_t i = 1; i < count; i++)
    {
        void* next = (unsigned char* const)ptr + size;

        if (compare_func(ptr, next) > 0)
        {
            return false;
        }

        ptr = next;
    }

    return true;
}

/*
** Bubble sorting
*/

void bubble_sort_d(double arr[static 1], size_t count)
{
    while (true)
    {
        bool no_change = true;

        for (size_t i = 1; i < count; i++)
        {
            if (arr[i - 1] > arr[i])
            {
                swap_doubles(&arr[i - 1], &arr[i]);
                no_change = false;
            }
        }

        if (no_change)
        {
            break;
        }

        --count; // the last element is in its place
    }
}

void bubble_sort_general(void* ptr, size_t count, const size_t size,
    int compare_func(const void*, const void*))
{
    while (true)
    {
        bool no_change = true;

        void* prev = ptr;
        for (size_t i = 1; i < count; i++)
        {
            void* next = (unsigned char* const)prev + size;

            if (compare_func(prev, next) > 0)
            {
                swap_voids(prev, next, size);
                no_change = false;
            }

            prev = next;
        }

        if (no_change)
        {
            break;
        }

        --count; // the last element is in its place, no need to check
    }
}

/*
** Merge sorting
*/

static void merge_d(double arr[static 1], const size_t left,
    const size_t middle, const size_t right)
{
    const size_t first_n = middle - left + 1;
    double first_subarray[first_n];
    for (size_t i = 0; i < first_n; ++i)
    {
        first_subarray[i] = arr[left + i];
    }

    const size_t second_n = right - middle;
    double second_subarray[second_n];
    for (size_t j = 0; j < second_n; ++j)
    
        second_subarray[j] = arr[middle + 1 + j];
    }

    size_t i = 0;
    size_t j = 0;
    size_t k = left;

    while ((i < first_n) && (j < second_n))
    {
#if 1
        arr[k++] = (first_subarray[i] < second_subarray[j])
                       ? first_subarray[i++]
                       : second_subarray[j++];
#else
        if (first_subarray[i] < second_subarray[j])
        {
            arr[k++] = first_subarray[i++];
        }
        else
        {
            arr[k++] = second_subarray[j++];
        }
#endif
    }

    while (i < first_n)
    {
        arr[k++] = first_subarray[i++];
    }

    while (j < second_n)
    {
        arr[k++] = second_subarray[j++];
    }
}

static void merge_sort_d_impl(
    const size_t left, const size_t right, double arr[static left + right + 1])
{
    if (left < right)
    {
        size_t middle = left + ((right - left) >> 1);

        merge_sort_d_impl(left, middle, arr);
        merge_sort_d_impl(middle + 1, right, arr);

        merge_d(arr, left, middle, right);
    }
}

void merge_sort_d(const size_t n, double arr[static n])
{
    merge_sort_d_impl(0, n - 1, arr);
}

static size_t partition_d(
    double array[static 1], const size_t left, const size_t right)
{
    const double pivot = array[right];

    size_t k = left;

    for (size_t i = left; i < right; ++i)
    {
        if (array[i] < pivot)
        {
            if (k < i)
            {
                swap_doubles(&array[i], &array[k]);
            }
            ++k;
        }
    }

    swap_doubles(&array[k], &array[right]);

    return k;
}

static void quick_sort_d_impl(
    const size_t left, const size_t right, double arr[static left + right + 1])
{
    if (left < right)
    {
        const size_t index = partition_d(arr, left, right);

        quick_sort_d_impl(left, index - 1, arr);
        quick_sort_d_impl(index + 1, right, arr);
    }
}

void quick_sort_d(const size_t n, double arr[static n])
{
    quick_sort_d_impl(0, n - 1, arr);
}
