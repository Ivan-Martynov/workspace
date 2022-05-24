#include <stdio.h>

#include "env_helper.h"

int main()
{
    printf("Attempt to displaying all environment variables:\n");

    const size_t count = list_variables();

    printf("Found %zu %s.\n", count,
           (count % 10 == 1) ? "variable" : "variables");

    printf("\nTry to find environment variables by name:\n");

    display_variable("TERM");
    display_variable("NONAME");

    return 0;
}