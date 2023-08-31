#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stddef.h>

double average_safe(const double, const double);

double average(const double, const double);

double sum_two(const double, const double);

double multiply_two(const double, const double);

double inc_by_dx(const double, const double);

double inc_by_two(const double);

double inc(const double);

double power_to_n(const double, const size_t);

double identity(const double);

double square(const double);

size_t gcd(int, int);
size_t lcm(int, int);

size_t gcd_array(const size_t n, int arr[static n]);
size_t lcm_array(const size_t n, int arr[static n]);

double repeated(double (*)(const double), size_t, double);

double smooth(double (*)(const double), const double, const double);

double smooth_n_fold(
    double (*)(const double), const size_t, const double, const double);

double deriv(double (*)(const double), const double x, const double);

#endif // _FUNCTIONS_H_