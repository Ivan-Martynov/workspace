#include "derivative_fun.h"

double newton(const double x, double (*f)(const double), const double h) {
	return (f(x + h) - f(x)) / h;
}

double central_diff_1(const double x, double (*f)(const double), const double h) {
	return (f(x + h) - f(x - h)) / (h + h);
}

double central_diff_2(const double x, double (*f)(double), const double h) {
	const double h2 = h + h;
	return (
		(f(x + h) - f(x - h)) * 8
		+ f(x - h2) - f(x + h2)
		) / (12 * h);
}

double central_diff_3(const double x, double (*f)(double), const double h) {
	const double h2 = h + h;
	const double h3 = h2 + h;
	return (
		(f(x + h) - f(x - h)) * 45
		+ (f(x - h2) - f(x + h2)) * 9
		+ f(x + h3) - f(x - h3)
		) / (60 * h);
}

double central_diff_4(const double x, double (*f)(double), const double h) {
	const double h2 = h + h;
	const double h3 = h2 + h;
	const double h4 = h2 + h2;
	return (
		(f(x + h) - f(x - h)) * 672
		+ (f(x - h2) - f(x + h2)) * 168
		+ (f(x + h3) - f(x - h3)) * 32
		+ (f(x - h4) - f(x + h4)) * 3) / (840 * h);
}

double complex
newton_c(
	const double complex x,
	double complex (*f)(const double complex),
	const double complex h) {
	return (f(x + h) - f(x)) / h;
}

