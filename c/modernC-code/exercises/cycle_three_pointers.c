#include <stdio.h>
#include <stdlib.h>

void swap_three(
    double* const first_ptr, double* const second_ptr, double* const third_ptr)
{
    const double temp = *first_ptr;
    *first_ptr = *second_ptr;
    *second_ptr = *third_ptr;
    *third_ptr = temp;
}

int main(void)
{
    double first_value = 3.62;
    double second_value = 4.0;
    double third_value = 18.042;

    printf("Before swap: %g; %g; %g\n", first_value, second_value, third_value);
    swap_three(&first_value, &second_value, &third_value);
    printf("First swap: %g; %g; %g\n", first_value, second_value, third_value);
    swap_three(&first_value, &second_value, &third_value);
    printf("Second swap: %g; %g; %g\n", first_value, second_value, third_value);
    swap_three(&first_value, &second_value, &third_value);
    printf("Third swap: %g; %g; %g\n", first_value, second_value, third_value);

    return EXIT_SUCCESS;
}
