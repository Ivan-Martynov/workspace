#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

double evaluate_guess_01(const double value)
{
    if (value >= 1)
    {
        double guess = value * 0.5;

        while (guess * guess > value)
        {
            guess *= 0.5;
        }

        return guess;
    }
    else
    {
        double guess = value * 2.0;

        while (guess * guess < value)
        {
            guess *= 2.0;
        }

        return guess;
    }
}

double heron_sqrt_guess_01(const double value, double guess, const double eps)
{
    while (fabs(guess * guess - value) >= eps)
    {
        guess = (guess + value / guess) * 0.5;
    }

    return guess;
}

void test_heron(void)
{
    const double precision = 1E-3;
    const double values[] = {0.0, 0.084, 1.0, 2.0, -34.0, 124948.3202};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    {
        const double value = values[i];

        if (value < 0)
        {
            continue;
        }

        const double guess = evaluate_guess_01(value);

        const double result = heron_sqrt_guess_01(value, guess, precision);

        printf("Result: %.12f ~ %.12f * %.12f = %.12f\n", value, result, result,
            result * result);
    }
}

int main(void) {
	test_heron();

	return EXIT_SUCCESS;
}
