#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#define MINSIZE(X, Y) (sizeof(X) < sizeof(Y) ? sizeof(X) : sizeof(Y))

#define BYTECOPY(T, S) memcpy(&(T), &(S), MINSIZE(T, S))

// Function and macro to make sure a string literal is passed as an argument.
static inline const char* string_literal(const char str[static 1])
{
    return str;
}
#define string_literal(S) string_literal("" S "")

// Simple macro to print or do nothing.
#if NDEBUG
#define TRACE_PRINT0(F, X)                                                     \
    do                                                                         \
    {                                                                          \
    } while (false)
#else
#define TRACE_PRINT0(F, X) fprintf(stderr, F, X)
#endif

// Macro to make sure format is a string literal. Additionally add a newline so
// that the user doesn't have to specify it.
#if NDEBUG
#define TRACE_PRINT1(F, X)                                                     \
    do                                                                         \
    {                                                                          \
    } while (false)
#else
#define TRACE_PRINT1(F, X) fprintf(stderr, "" F "\n", X)
#endif

#ifdef NDEBUG
#define TRACE_ON 0
#else
#define TRACE_ON 1
#endif

// Macro ensuring that the fprintf call is always evaluated.
#define TRACE_PRINT2(F, X)                                                     \
    do                                                                         \
    {                                                                          \
        if (TRACE_ON)                                                          \
        {                                                                      \
            fprintf(stderr, "" F "\n", X);                                       \
        }                                                                      \
    } while (false)

// Trace a value without specifying a format.
#define TRACE_VALUE0(HEAD, X) TRACE_PRINT2(HEAD " %Lg", (X) + 0.0L)

/**
 * @brief Trace a pointer without specifying a format.
 * 
 * @warning Uses a cast of X to void pointer.
 * 
 * @version 0.1
 * 
 * @date 2024-03-18
 */
#define TRACE_PTR0(HEAD, X) TRACE_PRINT2(HEAD " %p", (void*)(X))

// @brief Trace a pointer without specifying a format.
#define TRACE_PTR1(HEAD, X) TRACE_PRINT2(HEAD " %p", ((void*) {0} = (X)))

// Macro adding the current line number to the trace.
#define TRACE_PRINT3(F, X)                                                     \
    do                                                                         \
    {                                                                          \
        if (TRACE_ON)                                                          \
        {                                                                      \
            fprintf(stderr, "%lu: " F "\n", __LINE__ + 0UL, X);                \
        }                                                                      \
    } while (false)

// Macro adding the current function name and line number to the trace.
#define TRACE_PRINT4(F, X)                                                     \
    do                                                                         \
    {                                                                          \
        if (TRACE_ON)                                                          \
        {                                                                      \
            fprintf(stderr, "%s:%lu: " F "\n", __func__, __LINE__ + 0UL, X);   \
        }                                                                      \
    } while (false)

#define STRINGIFY(X) #X
#define STRGY(X) STRINGIFY(X)

// Adding a textual representation for __LINE__.
#define TRACE_PRINT5(F, X)                                                     \
    do                                                                         \
    {                                                                          \
        if (TRACE_ON)                                                          \
        {                                                                      \
            fprintf(stderr, "%s:" STRGY(__LINE__) ":(" #X "): " F "\n",        \
                __func__, X);                                                  \
        }                                                                      \
    } while (false)

// Print multiple arguments in the same trace.
#define TRACE_PRINT6(F, ...)                                                   \
    do                                                                         \
    {                                                                          \
        if (TRACE_ON)                                                          \
        {                                                                      \
            fprintf(stderr, "%s:" STRGY(__LINE__) ": " F "\n", __func__,       \
                __VA_ARGS__);                                                  \
        }                                                                      \
    } while (false)

// Only trace with a text message - no values printed.
#define TRACE_PRINT7(...)                                                      \
    do                                                                         \
    {                                                                          \
        if (TRACE_ON)                                                          \
        {                                                                      \
            fprintf(stderr, "%s:" STRGY(__LINE__) ": " __VA_ARGS__ "\n",       \
                __func__);                                                     \
        }                                                                      \
    } while (false)

#define TRACE_FIRST_0(X, ...) X
#define TRACE_FIRST(...) TRACE_FIRST_0(__VA_ARGS__, 0)

#define TRACE_LAST_0(X, ...) __VA_ARGS__
#define TRACE_LAST(...) TRACE_LAST_0(__VA_ARGS__, 0)

// Only trace with a text message - no values printed.
#define TRACE_PRINT8(...)                                                      \
    TRACE_PRINT6(TRACE_FIRST(__VA_ARGS__) "%.0d", TRACE_LAST(__VA_ARGS__))

// Trace with textual representation of the arguments.
#define TRACE_PRINT9(F, ...) TRACE_PRINT6("(" #__VA_ARGS__ ") " F, __VA_ARGS__)

// Call a three-parameter function with default arguments set to zero.
#define ZERO_DEFAULT3_0(FUNC, X, Y, Z, ...) FUNC(X, Y, Z)
#define ZERO_DEFAULT3(FUNC, ...) ZERO_DEFAULT3_0(FUNC, __VA_ARGS__, 0, 0, )

#define strtoul(...) ZERO_DEFAULT3(strtoul, __VA_ARGS__)
#define strtoull(...) ZERO_DEFAULT3(strtoull, __VA_ARGS__)
#define strtol(...) ZERO_DEFAULT3(strtol, __VA_ARGS__)

// Skip initial part of the argument.
static inline const char* trace_skip(const char expr[static 1])
{
    for (const char* p = expr + 1; *p; ++p)
    {
        // End of head expects the 3 chars ``", ''
        if (p[0] == '"')
        {
            return p + 3;
        }

        // Watch out for escaped " characters
        if ((p[0] == '\\') && (p[1] == '"'))
        {
            ++p;
        }
    }

    return 0;
}

/**
 * @brief Function to print a list of values.
 *
 * @param stream
 * @param func
 * @param line
 * @param expr
 * @param head
 * @param len
 * @param arr
 *
 * @remark Only call this function through the macro ::TRACE_VALUES, which will
 * provide the necessary contextual information.
 *
 * @version 0.1
 *
 * @date 2024-03-19
 */
static inline void trace_values(FILE* stream,
    const char func[restrict static 1], const char line[restrict static 1],
    const char expr[restrict static 1], const char head[restrict static 1],
    const size_t len, const long double arr[static len])
{
    fprintf(stream, "%s:%s:(%s) %s %Lg", func, line, trace_skip(expr), head,
        arr[0]);

    for (size_t i = 1; i < len - 1; ++i)
    {
        fprintf(stream, ", %Lg", arr[i]);
    }

    fprintf(stream, "\n");
}

#define ALEN_0(_00, _01, _02, _03, _04, _05, _06, _07, _08, _09, _0A, _0B,     \
    _0C, _0D, _0E, _0F, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1A, \
    _1B, _1C, _1D, _1E, _1F, ...)                                              \
    _1F

// Number of arguments in the ... list. This version works for lists with up to
// 32 elements. An empty argument list is taken as one (empty argument)
#define ALEN(...)                                                              \
    ALEN_0(__VA_ARGS__, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17,  \
        0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C,      \
        0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,      \
        0x00)

#define TRACE_VALUES_0(NARGS, EXPR, HEAD, ...)                                 \
    do                                                                         \
    {                                                                          \
        if (TRACE_ON)                                                          \
        {                                                                      \
            if (NARGS > 1)                                                     \
            {                                                                  \
                trace_values(stderr, __func__, STRGY(__LINE__), "" EXPR "",    \
                    "" HEAD "", NARGS,                                         \
                    (long double const[NARGS]) {__VA_ARGS__});                 \
            }                                                                  \
            else                                                               \
            {                                                                  \
                fprintf(                                                       \
                    stderr, "%s:" STRGY(__LINE__) ": %s\n", __func__, HEAD);   \
            }                                                                  \
        }                                                                      \
    } while (false)

#define TRACE_VALUES(...)                                                      \
    TRACE_VALUES_0(ALEN(__VA_ARGS__), #__VA_ARGS__, __VA_ARGS__, 0)

static inline double sum_array(const size_t n, const double arr[static n])
{
    double result = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        result += arr[i];
    }

    return result;
}
#define SUM_VALUES(...)                                                        \
    sum_array(                                                                 \
        ALEN(__VA_ARGS__), (double const[ALEN(__VA_ARGS__)]) {__VA_ARGS__})

FILE* g_io_debug = NULL;

// Print to the debug stream @g_io_debug.
#ifdef __GNUC__
__attribute__((format(printf, 1, 2)))
#endif

static int print_debug(const char* const format, ...)
{
    int result = 0;

    if (g_io_debug)
    {
        va_list args_list;
        va_start(args_list, format);
        result = vfprintf(g_io_debug, format, args_list);
        va_end(args_list);
    }

    return result;
}

static double sum_arr_variadic(const size_t n, ...)
{
    double result = 0.0;

    va_list args_list;
    va_start(args_list, n);
    for (size_t i = 0; i < n; ++i)
    {
        result += va_arg(args_list, double);
    }
    va_end(args_list);

    return result;
}

static void test_values(void)
{
    int x = 3;
    TRACE_VALUE0("Value", x);
    TRACE_PTR1("Pointer", &x);

    // TRACE_PRINT6("Checking values: %d; %d; %g", x, x * 4, x + 3.2);
    TRACE_PRINT8("Just a string.");
    TRACE_PRINT8("Checking values: %d; %d; %g", x, x * 4, x + 3.2);
    TRACE_PRINT9("Checking values: %d; %d; %g", x, x * 4, x + 3.2);

    const double sum = SUM_VALUES(x, x * 4, x + 3.2);
    TRACE_VALUES("", x, x * 4, x + 3.2, sum);

    print_debug("Test %g\n", sum_arr_variadic(4, 12.2, 3.9, -23.898, 12043));
}

static void test_string(const char* const text)
{
#if 0
    // Compilation fails.
    printf("Text = %s\n", string_literal(text));
#endif

    if (text)
    {
        printf("String = %s\n", string_literal("Test string."));
#if 1
        const unsigned long value = strtoul(text);
        printf("Trying to convert text \"%s\" to number => %lu\n", text, value);
#endif
    }
    else
    {
        printf("String = %s\n", ""
                                "Test string."
                                "");
    }

#if 0
    // Compilation fails.
    char s[] = "Another string.";
    printf("%s\n", string_literal(s));
#endif
}

int main(const int argc, const char* const argv[static argc])
{
    if (argc > 1)
    {
        test_string(argv[1]);
    }
    else
    {
        test_string(NULL);
        test_values();
    }

    return 0;
}
