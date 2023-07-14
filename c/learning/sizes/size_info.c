#include "size_info.h"

#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

void show_integer_limits()
{
    printf("\nCHAR_BIT = %d; number of bits in a byte.\n", CHAR_BIT);
    printf("\nMB_LEN_MAX = %d; maximum number of bytes in a multibyte "
           "character.\n",
        MB_LEN_MAX);

    printf("\nCHAR_MIN = %+d; minimum value of char.\n", CHAR_MIN);
    printf("\nCHAR_MAX = %+d; maximum value of char.\n", CHAR_MAX);
    printf("\nSCHAR_MIN = %+d; minimum value of signed char.\n", SCHAR_MIN);
    printf("\nSCHAR_MAX = %+d; minimum value of signed char.\n", SCHAR_MAX);
    printf("\nUCHAR_MAX = %u; maximum value of unsigned char.\n", UCHAR_MAX);

    printf("\nSHRT_MIN = %+d; minimum value of signed short.\n", SHRT_MIN);
    printf("\nSHRT_MAX = %+d; maximum value of signed short.\n", SHRT_MAX);
    printf("\nUSHRT_MAX = %u; maximum value of unsigned short.\n", USHRT_MAX);

    printf("\nINT_MIN = %+d; minimum value of signed int.\n", INT_MIN);
    printf("\nINT_MAX = %+d; maximum value of signed int.\n", INT_MAX);
    printf("\nUINT_MAX = %u; maximum value of unsigned int.\n", UINT_MAX);

    printf("\nLONG_MIN = %+ld; minimum value of signed long.\n", LONG_MIN);
    printf("\nLONG_MAX = %+ld; maximum value of signed long.\n", LONG_MAX);
    printf("\nULONG_MAX = %lu; maximum value of unsigned long.\n", ULONG_MAX);

    printf(
        "\nLLONG_MIN = %+lld; minimum value of signed long long.\n", LLONG_MIN);
    printf(
        "\nLLONG_MAX = %+lld; maximum value of signed long long.\n", LLONG_MAX);
    printf("\nULLONG_MAX = %llu; maximum value of unsigned long long.\n",
        ULLONG_MAX);

    printf("\nPTRDIFF_MIN = %td; minimum value of ptrdiff_t.\n", PTRDIFF_MIN);
    printf("\nPTRDIFF_MAX = %+td; maximum value of ptrdiff_t.\n", PTRDIFF_MAX);
    printf("\nSIZE_MAX = %zu; maximum value of size_t.\n", SIZE_MAX);
    printf("\nSIG_ATOMIC_MIN = %+jd; minimum value of sig_atomic_t.\n",
        (intmax_t)SIG_ATOMIC_MIN);
    printf("\nSIG_ATOMIC_MAX = %+jd; maximum value of sig_atomic_t.\n",
        (intmax_t)SIG_ATOMIC_MAX);
    printf(
        "\nWCHAR_MIN = %+jd; minimum value of wchar_t.\n", (intmax_t)WCHAR_MIN);
    printf(
        "\nWCHAR_MAX = %+jd; maximum value of wchar_t.\n", (intmax_t)WCHAR_MAX);
    printf("\nWINT_MIN = %jd; minimum value of wint_t.\n", (intmax_t)WINT_MIN);
    printf("\nWINT_MAX = %jd; maximum value of wint_t.\n", (intmax_t)WINT_MAX);
}

void show_float_limits()
{
    printf("\nFLT_RADIX = %d; the radix (integer base) used by the "
           "representation of all three floating-point types.\n",
        FLT_RADIX);

    printf("\nDECIMAL_DIG = %d; conversion from long double to decimal with "
           "at least DECIMAL_DIG digits and back to long double is the "
           "identity conversion: this is the decimal precision required to "
           "serialize/deserialize a long double.\n",
        DECIMAL_DIG);

    printf(
        "\nFLT_DECIMAL_DIG = %d; conversion from float/double/long double to "
        "decimal with at least FLT_DECIMAL_DIG digits and back is the "
        "identity conversion.\n",
        FLT_DECIMAL_DIG);
    printf(
        "\nDBL_DECIMAL_DIG = %d; conversion from float/double/long double to "
        "decimal with at least DBL_DECIMAL_DIG digits and back is the "
        "identity conversion.\n",
        DBL_DECIMAL_DIG);
    printf(
        "\nLDBL_DECIMAL_DIG = %d; conversion from float/double/long double to "
        "decimal with at least LDBL_DECIMAL_DIG digits and back is the "
        "identity conversion.\n",
        LDBL_DECIMAL_DIG);

    printf("\nFLT_MIN = %e; minimum, normalized, positive value of float.\n",
        FLT_MIN);
    printf("\nDBL_MIN = %e; minimum, normalized, positive value of "
           "double.\n",
        DBL_MIN);
    printf("\nLDBL_MIN = %e; minimum, normalized, positive value of long "
           "double.\n",
        LDBL_MIN);

    printf("\nFLT_TRUE_MIN = %e; minimum positive value of float.\n",
        FLT_TRUE_MIN);
    printf("\nDBL_TRUE_MIN = %e; minimum positive value of double.\n",
        DBL_TRUE_MIN);
    printf("\nLDBL_TRUE_MIN = %e; minimum positive value of long double.\n",
        LDBL_TRUE_MIN);

    printf("\nFLT_MAX = %e; maximum finite value of float.\n", FLT_MAX);
    printf("\nDBL_MAX = %e; maximum finite value of double.\n", DBL_MAX);
    printf("\nLDBL_MAX = %e; maximum finite value of long double.\n", LDBL_MAX);

    printf("\nFLT_EPSILON = %e; absolute value difference between 1.0 and the "
           "next representable value for float.\n",
        FLT_EPSILON);
    printf("\nDBL_EPSILON = %e; absolute value difference between 1.0 and the "
           "next representable value for double.\n",
        DBL_EPSILON);
    printf("\nLDBL_EPSILON = %e; absolute value difference between 1.0 and the "
           "next representable value for long double.\n",
        LDBL_EPSILON);

    printf("\nFLT_DIG = %d; number of decimal digits that are guaranteed to be "
           "preserved in text → float → text roundtrip without change due to "
           "rounding or overflow.\n",
        FLT_DIG);
    printf("\nDBL_DIG = %d; number of decimal digits that are guaranteed to be "
           "preserved in text → double → text roundtrip without change due to "
           "rounding or overflow.\n",
        DBL_DIG);
    printf("\nLDBL_DIG = %d; number of decimal digits that are guaranteed to "
           "be preserved in text → long double → text roundtrip without change "
           "due to rounding or overflow.\n",
        LDBL_DIG);

    printf("\nFLT_MANT_DIG = %d; number of base-FLT_RADIX digits that are in "
           "the floating-point mantissa and that can be represented without "
           "losing precision for float.\n",
        FLT_MANT_DIG);
    printf("\nDBL_MANT_DIG = %d; number of base-FLT_RADIX digits that are in "
           "the floating-point mantissa and that can be represented without "
           "losing precision for double.\n",
        DBL_MANT_DIG);
    printf("\nLDBL_MANT_DIG = %d; number of base-FLT_RADIX digits that are in "
           "the floating-point mantissa and that can be represented without "
           "losing precision for long double.\n",
        LDBL_MANT_DIG);

    printf(
        "\nFLT_MIN_EXP = %d; minimum negative integer such that FLT_RADIX "
        "raised by power one less than that integer is a normalized float.\n",
        FLT_MIN_EXP);
    printf(
        "\nDBL_MIN_EXP = %d; minimum negative integer such that FLT_RADIX "
        "raised by power one less than that integer is a normalized double.\n",
        DBL_MIN_EXP);
    printf("\nLDBL_MIN_EXP = %d; minimum negative integer such that FLT_RADIX "
           "raised by power one less than that integer is a normalized long "
           "double.\n",
        LDBL_MIN_EXP);

    printf(
        "\nFLT_MIN_10_EXP = %d; minimum negative integer such that 10 raised "
        "by power one less than that integer is a normalized float.\n",
        FLT_MIN_10_EXP);
    printf(
        "\nDBL_MIN_10_EXP = %d; minimum negative integer such that 10 raised "
        "by power one less than that integer is a normalized double.\n",
        DBL_MIN_10_EXP);
    printf(
        "\nLDBL_MIN_10_EXP = %d; minimum negative integer such that 10 raised "
        "by power one less than that integer is a normalized long double.\n",
        LDBL_MIN_10_EXP);

    printf("\nFLT_MAX_EXP = %d; maximum positive integer such that FLT_RADIX "
           "raised by power one less than that integer is a representable "
           "finite float.\n",
        FLT_MAX_EXP);
    printf("\nDBL_MAX_EXP = %d; maximum positive integer such that FLT_RADIX "
           "raised by power one less than that integer is a representable "
           "finite double.\n",
        DBL_MAX_EXP);
    printf("\nLDBL_MAX_EXP = %d; maximum positive integer such that FLT_RADIX "
           "raised by power one less than that integer is a representable "
           "finite long double.\n",
        LDBL_MAX_EXP);

    printf("\nFLT_MAX_10_EXP = %d; maximum positive integer such that 10 "
           "raised by power one less than that integer is a representable "
           "finite float.\n",
        FLT_MAX_10_EXP);
    printf("\nDBL_MAX_10_EXP = %d; maximum positive integer such that 10 "
           "raised by power one less than that integer is a representable "
           "finite double.\n",
        DBL_MAX_10_EXP);
    printf("\nLDBL_MAX_10_EXP = %d; maximum positive integer such that 10 "
           "raised by power one less than that integer is a representable "
           "finite long double.\n",
        LDBL_MAX_10_EXP);

    printf("\nFLT_ROUNDS = %d; rounding mode of floating-point arithmetic: "
           "-1 - the default rounding direction is not known; 0 - toward zero; "
           "same meaning as FE_TOWARDZERO; 1 - to nearest; same meaning as "
           "FE_TONEAREST;2 - towards positive infinity; same meaning as "
           "FE_UPWARD; 3 - towards negative infinity; same meaning as "
           "FE_DOWNWARD; "
           "other values - implementation-defined behavior .\n",
        FLT_ROUNDS);
    printf("\nFLT_EVAL_METHOD = %d; use of extended precision for intermediate "
           "results: 0 - not used, 1 - double is used instead of float, "
           "2 - long double is used.\n",
        FLT_EVAL_METHOD);

    printf(
        "\nFLT_HAS_SUBNORM = %d; whether float supports subnormal (denormal) "
        "numbers: -1 - indeterminable, 0 - absent, 1 - present .\n",
        FLT_HAS_SUBNORM);
    printf(
        "\nDBL_HAS_SUBNORM = %d; whether double supports subnormal (denormal) "
        "numbers: -1 - indeterminable, 0 - absent, 1 - present .\n",
        DBL_HAS_SUBNORM);
    printf(
        "\nLDBL_HAS_SUBNORM = %d; whether long double supports subnormal "
        "(denormal) numbers: -1 - indeterminable, 0 - absent, 1 - present .\n",
        LDBL_HAS_SUBNORM);
}
