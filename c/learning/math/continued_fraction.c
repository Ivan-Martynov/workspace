#include <stdio.h>

double coninued_fraction_recursive_helper(double n_func(const size_t),
    double d_func(const size_t), const size_t k, const size_t i)
{
    if (i >= k)
    {
        return n_func(k) / d_func(k);
    }
    else
    {
        return n_func(i)
               / (d_func(i)
                   + coninued_fraction_recursive_helper(
                       n_func, d_func, k, i + 1));
    }
}

double coninued_fraction_recursive(double n_func(const size_t),
    double d_func(const size_t), size_t k)
{
    return coninued_fraction_recursive_helper(n_func, d_func, k, 1);
}

double coninued_fraction_iterative(double n_func(const size_t),
    double d_func(const size_t), size_t k)
{
    double result = 0;

    for (; k > 0; --k)
    {
        result = n_func(k) / (d_func(k) + result);
    }

    return result;
}

double coninued_fraction(double n_func(const size_t),
    double d_func(const size_t), size_t k)
{
#if 1
    return coninued_fraction_iterative(n_func, d_func, k);
#else
    return coninued_fraction_recursive(n_func, d_func, k);
#endif
}

double n_func_one(const size_t i)
{
    (void)i;
    return 1.0;
}

double d_func_one(const size_t i)
{
    (void)i;
    return 1.0;
}

double d_func_euler_step(const size_t i)
{
    return ((i + 1) % 3 == 0) ? ((i + 1) / 3) << 1 : 1;
}

double exp_approx(const size_t n)
{
    return 2 + coninued_fraction(n_func_one, d_func_euler_step, n);
}

int main()
{
    printf("1/phi ~= %f\n", coninued_fraction(n_func_one, d_func_one, 7));
    printf("e ~= %f\n", exp_approx(7));
    return 0;
}