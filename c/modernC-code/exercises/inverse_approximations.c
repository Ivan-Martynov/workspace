#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

double heron_inverse_guess_01(const double value, double guess, const double eps) {
	while (true) {
		const double product = value * guess;

		if (fabs(product - 1.0) < eps) {
			break;
		} else {
			guess *= 2.0 - product;
		}
	}

	return guess;
}

double heron_approx_inverse_01(const double value, const double eps) {
	double guess = 1.0;

    while (true) {
		double prod = value * guess;
		if (prod < 1.0 - 1E-01) {
			guess *= 2.0;
		} else if (prod > 1.0 + 1E-01) {
			guess *= 0.5;
		} else {
			break;
		}
    }

	return heron_inverse_guess_01(value, guess, eps);
}

void test_heron(void) {
	const double value = 34.0;
	const double precision = 1E-3;

	const double result = heron_approx_inverse_01(value, precision);

	printf("Result: %f * %f = %f\n", value, result, value * result);
}

int main(void) {
	test_heron();

	return EXIT_SUCCESS;
}
