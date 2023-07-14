#include <stdio.h>

/**
 * @brief Calculate a factorial using a recursive procedure.
 * 
 * @param n Value to compute a factoral of.
 * 
 * @return size_t Factorial.
 * 
 * @version 0.1
 * 
 * @author Ivan Martynov
 * 
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 */
size_t factorial_recursive(const size_t n)
{
    return (n < 2) ? 1 : n * factorial_recursive(n - 1);
}

/**
 * @brief Calculate a factorial using an iterative procedure.
 *
 * @remark The argument is not const, because it is used for computation in the
 * procedure and its value changes during the iteration process.
 *
 * @param n Value to compute a factoral of.
 *
 * @return size_t Factorial.
 *
 * @version 0.1
 *
 * @author Ivan Martynov
 *
 * @date 2023-07-14
 *
 * @copyright Copyright (c) 2023
 */
size_t factorial_iterative(size_t n)
{
    size_t result = 1;

    for (; n > 1; --n)
    {
        result *= n;
    }

    return result;
}

void test_factorial_functions()
{
    const size_t array[] = {0, 5, 2, 1, 3};
    const size_t count = sizeof(array) / sizeof(array[0]);

    printf("Testing factorial functions\n");
    for (size_t i = 0; i < count; ++i)
    {
        const size_t n = array[i];

        printf("\nfactorial_recursive(%zu) = %zu\n", n, factorial_recursive(n));
        printf("factorial_iterative(%zu) = %zu\n", n, factorial_iterative(n));
    }
}

int main()
{
    test_factorial_functions();

    return 0;
}