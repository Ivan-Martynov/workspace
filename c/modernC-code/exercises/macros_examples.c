#include <stdio.h>
#include <stdbool.h>

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

static void test_values(void)
{
    int x = 3;
    TRACE_VALUE0("Value", x);
    TRACE_PTR1("Pointer", &x);

    TRACE_PRINT5("Checking integer: %d", x * 4);
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
