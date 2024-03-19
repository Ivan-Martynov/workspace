#include <stdio.h>
#include <complex.h>
#include <limits.h>
#include <float.h>
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

// Maximum value for the type of @X.
#define SIGNEDNESS(X) _Generic((X),             \
    bool: false,                                \
    char: true                                  \
    signed char: true                           \
    unsigned char: false                        \
    signed short: true                          \
    unsigned short: false                       \
    signed int: true                            \
    unsigned int: false                         \
    signed long: true                           \
    unsigned long: false                        \
    signed long long: true                      \
    unsigned long long: false                   \
    float: true                                 \
    double: true                                \
    long double: true)

#define MAX_WITH_SIGN(A, B)                                                    \
    do                                                                         \
    {                                                                          \
        if (SIGNEDNESS(A) == SIGNEDNESS(B))                                    \
        {                                                                      \
            if (A < B)                                                         \
            {                                                                  \
                PROMOTE((A) < (B) ? (A) : (B), A);                             \
            }                                                                  \
            else                                                               \
            {                                                                  \
                PROMOTE((A) < (B) ? (A) : (B), B);                             \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            if (A < B)                                                         \
            {                                                                  \
                PROMOTE(abs(A) < abs(B) ? abs(A) : abs(B), A);                 \
            }                                                                  \
            else                                                               \
            {                                                                  \
                PROMOTE(abs(A) < abs(B) ? abs(A) : abs(B), B);                 \
            }                                                                  \
        }                                                                      \
    } while (false)

// Maximum value for the type of @X.
#define MAX_VAL(X) _Generic((X),                \
    bool: (bool)+1,                             \
    char: (char)+CHAR_MAX                       \
    signed char: (signed char)+SCHAR_MAX        \
    unsigned char: (unsigned char)+UCHAR_MAX    \
    signed short: (signed short)+SHRT_MAX       \
    unsigned short: (unsigned short)+USHRT_MAX  \
    signed int: INT_MAX                         \
    unsigned int: UINT_MAX                      \
    signed long: LONG_MAX                       \
    unsigned long: ULONG_MAX                    \
    signed long long: LLONG_MAX                 \
    unsigned long long: ULLONG_MAX              \
    float: FLT_MAX                              \
    double: DBL_MAX                             \
    long double: LDBL_MAX)

// Minimum value for the type of @X.
#define MIN_VAL(X) _Generic((X),                \
    bool: (bool)+0,                             \
    char: (char)+CHAR_MIN                       \
    signed char: (signed char)+SCHAR_MIN        \
    unsigned char: (unsigned char)+UCHAR_MIN    \
    signed short: (signed short)+SHRT_MIN       \
    unsigned short: (unsigned short)+USHRT_MIN  \
    signed int: INT_MIN                         \
    unsigned int: UINT_MIN                      \
    signed long: LONG_MIN                       \
    unsigned long: ULONG_MIN                    \
    signed long long: LLONG_MIN                 \
    unsigned long long: ULLONG_MIN              \
    float: FLT_MIN                              \
    double: DBL_MIN                             \
    long double: LDBL_MIN

#define MAX_OF(T) _Generic(0 + (T) + 0,         \
    signed int: INT_MAX                         \
    unsigned int: UINT_MAX                      \
    signed long: LONG_MAX                       \
    unsigned long: ULONG_MAX                    \
    signed long long: LLONG_MAX                 \
    unsigned long long: ULLONG_MAX              \
    float: FLT_MAX                              \
    double: DBL_MAX                             \
    long double: LDBL_MAX)

#define MIN_OF(T) _Generic(0 + (T) + 0,         \
    signed int: INT_MIN                         \
    unsigned int: UINT_MIN                      \
    signed long: LONG_MIN                       \
    unsigned long: ULONG_MIN                    \
    signed long long: LLONG_MIN                 \
    unsigned long long: ULLONG_MIN              \
    float: FLT_MIN                              \
    double: DBL_MIN                             \
    long double: LDBL_MIN)

#define PROMOTE(T, A) _Generic((T)              \
    bool: (bool)                                \
    char: (char)                                \
    signed char: (signed char)                  \
    unsigned char: (unsigned char)              \
    signed short: (signed short)                \
    unsigned short: (unsigned short)            \
    signed int: (signed int)                    \
    unsigned int: (unsigned int)                \
    signed long: (signed long)                  \
    unsigned long: (unsigned long)              \
    signed long long: (signed long long)        \
    unsigned long long: (unsigned long long)    \
    float: (float)                              \
    double: (double)                            \
    long double: (long double))(A)

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
