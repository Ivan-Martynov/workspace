#ifndef _DERIVATIVE_FUN_H_
#define _DERIVATIVE_FUN_H_

#include <complex.h>

double newton(const double, double (*)(const double), const double);
double central_diff_1(const double, double (*)(const double), const double);
double central_diff_2(const double x, double (*)(double), const double);
double central_diff_3(const double x, double (*)(double), const double);
double central_diff_4(const double x, double (*)(double), const double);

double complex newton_c(const double complex,
    double complex (*)(const double complex), const double complex);

#endif