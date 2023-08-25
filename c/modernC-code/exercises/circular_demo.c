#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circular.h"

void printf_size_t_array(const size_t n, const size_t arr[static n])
{
    if (!arr)
    {
        printf("Array is null.\n");
    }

    if (!n)
    {
        printf("Provided zero number of elements.\n");
    }

    printf("%zu", arr[0]);
    for (size_t i = 1; i<n;++i)
    {
        printf(" %zu", arr[i]);
    }
    printf("\n");
}


void test_array(void)
{
    size_t arr[] = {[2] = 23, [3] = 48, [4] = 78, [10] = 0};
    const size_t len = sizeof(arr) / sizeof(arr[0]);

    printf("Before memmove:\n");
    printf_size_t_array(len, arr);

    memmove(arr + 4, arr + 2, 3 * sizeof(arr[0]));

    printf("After memmove:\n");
    printf_size_t_array(len, arr);
}

int main(void)
{
    size_t len = 2;
    circular* c = circular_new(len);

    circular_append(c, 10.23);
    circular_append(c, 0.94);
    circular_append(c, -3.021);

    circular_fput(c, stdout);

    circular_delete(c);

    test_array();

    return EXIT_SUCCESS;
}