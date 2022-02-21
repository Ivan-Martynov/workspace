#include <stdio.h>
#include <time.h>

#include "../printmode/printmode.h"

void wait_for_time(const double seconds)
{
    printf("Hey, Marvin! How many sheeps can you count in %f second(s)?\n",
           seconds);

    long int clock_count = 0L;
    long int limit = (long int)(seconds * CLOCKS_PER_SEC) + 1;
    long int sheep_count = 0L;

    while (clock_count < limit)
    {
        clock_t start = clock();
        ++sheep_count;
        clock_count += clock() - start;
    }

    printf("Pfft, you have no idea... at least ");
    set_printmode_fg(BLUE_C);
    printf("%ld ", sheep_count);
    reset_printmode();
    puts("sheeps.");
}

void time_to_loop(const size_t n)
{
    printf("Hey, Marvin! How much time does it take to loop through %zu "
           "elements?\n",
           n);

    long int clock_count = 0L;

    for (size_t i = 0; i < n; ++i)
    {
        clock_t start = clock();
        //printf("Element %zu\n", i);
        clock_count += clock() - start;
    }

    printf("Bummer, it takes nothing for me. To be precise ");
    set_printmode_fg(BLUE_C);
    printf("%ld ", clock_count);
    reset_printmode();
    printf("clocks or ");
    set_printmode_fg(BLUE_C);
    printf("%f ", (double)clock_count / CLOCKS_PER_SEC);
    reset_printmode();
    puts("seconds.");
}

int main(void)
{
    const size_t n = 1000;
    time_to_loop(n);

    const double seconds = 0.333;
    wait_for_time(seconds);

    return 0;
}

