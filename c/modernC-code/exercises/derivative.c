#include <stdio.h>
#include <tgmath.h>

#include "derivative_fun.h"

double cube(const double x) {
	return pow(x, 3);
}

int main() {
	const double value = 4;
	double (*fun)(double) = &cube;
	printf("%f\n", fun(value));

	const double h = 0.01;

	double a1 = newton(value, fun, h);
	double a2 = central_diff_1(value, fun, h);
	double a3 = central_diff_2(value, fun, h);
	double a4 = central_diff_3(value, fun, h);
	double a5 = central_diff_4(value, fun, h);

	printf("%.8f\n", a1);
	printf("%.8f\n", a2);
	printf("%.8f\n", a3);
	printf("%.8f\n", a4);
	printf("%.8f\n", a5);

	const double pi = acos(-1);
	const double complex c = I * pi;
	double complex(*fc)(double complex) = &cexp;
	const double complex hc = 0.01 + 0.01 * I;

	double complex b = fc(c);
	double complex b1 = newton_c(c, fc, hc);
	printf("%.8f + %.8fi\n", creal(b), cimag(b));
	printf("%.8f + %.8fi\n", creal(b1), cimag(b1));

	return 0;
}
