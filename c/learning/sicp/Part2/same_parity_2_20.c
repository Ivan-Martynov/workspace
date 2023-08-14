#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

struct parity_pair_array
{
    int* first_array;
    int* second_array;

    size_t first_count;
    size_t second_count;
};

struct parity_pair_array same_parity(const size_t count, ...)
{
    va_list args;
    va_start(args, count);

    const int first = va_arg(args, int);
    const int first_parity = first % 2;

    struct parity_pair_array parity_struct = {.first_array = realloc(NULL, 1),
        .second_array = NULL,
        .first_count = 1,
        .second_count = 0};
    parity_struct.first_array[0] = first;

    for (size_t i = 1; i < count; ++i)
    {
        const int value = va_arg(args, int);
        if (value % 2 == first_parity)
        {
            ++parity_struct.first_count;
            parity_struct.first_array
                = realloc(parity_struct.first_array, parity_struct.first_count);
            parity_struct.first_array[parity_struct.first_count - 1] = value;
        }
        else
        {
            ++parity_struct.second_count;
            parity_struct.second_array = realloc(
                parity_struct.second_array, parity_struct.second_count);
            parity_struct.second_array[parity_struct.second_count - 1] = value;
        }
    }

    va_end(args);

    return parity_struct;
}

int main()
{
    struct parity_pair_array parity_arrays = same_parity(5, 1, 7, 4, 9, 2);

    printf("First array: ");
    for (size_t i = 0; i < parity_arrays.first_count; ++i)
    {
        printf("%d ", parity_arrays.first_array[i]);
    }
    printf("\n");

    printf("Second array: ");
    for (size_t i = 0; i < parity_arrays.second_count; ++i)
    {
        printf("%d ", parity_arrays.second_array[i]);
    }
    printf("\n");

    free(parity_arrays.first_array);
    free(parity_arrays.second_array);

    return 0;
}
