#include <stdio.h>
#include <math.h>

double method_1(const size_t n) {
	double result = 0;

	for (size_t i = 0; i < n; ++i) {
		const size_t k = i << 3;

		result += (4.0 / (k + 1) - 2.0 / (k + 4) - 1.0 / (k + 5) - 1.0 / (k + 6))
			/ powf(16.0, i);
	}

	return result;
}

double method_2(const size_t n) {
	double result = 0;

	for (size_t i = 0; i < n; ++i) {
		const size_t k = i * 4;
		const size_t m = i * 10;

		const double sum =
			1.0   / (m + 9) -
			4.0   / (m + 7) -
			4.0   / (m + 5) -
			64.0  / (m + 3) +
			256.0 / (m + 1) -
			1.0   / (k + 3) -
			32.0  / (k + 1);

		const double t = sum / pow(2, m);

		if (i % 2 == 0) {
			result += t;
		} else {
			result -= t;
		}
	}

	return result / 64;
}

int main() {
	const size_t n = 2;
	const double a = method_1(n);
	const double b = method_2(n);
	printf("%.16f\n", a);
	printf("%.16f\n", b);

	return 0;
}
