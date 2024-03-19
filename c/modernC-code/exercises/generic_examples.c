#include <stdio.h>
#include <complex.h>
#include <math.h>

//#if defined(__STDC_IEC_559_COMPLEX__) || defined(__STDC_IEC_60559_COMPLEX__)
#if 1
#define abs(X)                                                                 \
    _Generic((X),\
    float: fabsf,\
    long double: fabsl,\
    float complex: cabsf,\
    double complex: cabs,\
    long double complex: cabsl,\
    default: fabs)(X)
#else
#define abs(X)                                                                \
    _Generic((X), float : fabsf, long double : fabsl, default : fabs)(X)
#endif

static inline double min(const double a, const double b)
{
    return a < b ? a : b;
}

static inline long double minl(const long double a, const long double b)
{
    return a < b ? a : b;
}

static inline float minf(const float a, const float b)
{
    return a < b ? a : b;
}

static inline size_t min_size_t(const size_t a, const size_t b)
{
    return a < b ? a : b;
}

#define MIN_PTR_IMPL(PTR_1, PTR_2) ((PTR_1) < (PTR_2) ? (PTR_1) : (PTR_2) )
#define MIN_PTR(A, B) MIN_PTR_IMPL((void*) {0} = (A), (void*) {0} = (B))

static inline void* minp(void* a_ptr, void* b_ptr)
{
    return MIN_PTR(a_ptr, b_ptr);
}

#define min(A, B)       \
    _Generic((B) - (A), \
    float: minf,        \
    double: min,        \
    long double: minl,  \
    size_t: min_size_t, \
    default: minp) ((A), (B))

int main()
{
    double complex z = 1.0 + 1.0*I;
    printf("%.1f%+.1fi cartesian is rho=%f theta=%f polar\n",
           creal(z), cimag(z), abs(z), carg(z));
    
    float a_f = 2.3f;
    float b_f = -5.2f;
    printf("Min of (%f, %f) = %f\n", a_f, b_f, min(a_f, b_f));
    printf("Min of pointers (%p, %p) = %p\n", (void*)&a_f, (void*)&b_f,
        min(&a_f, &b_f));
    size_t a_zu = 3;
    size_t b_zu = 5;
    printf("Min of (%zu, %zu) = %zu\n", a_zu, b_zu, min(a_zu, b_zu));

    return 0;
}
