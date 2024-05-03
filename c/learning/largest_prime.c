/** @file largestPrime.c
The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600851475143 ?
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// uncomment to disable assert()
#define NDEBUG
#include <assert.h>

#include <math.h>

#if __STDC_NO_THREADS__
#error "No threads support provided."
#endif

#include <threads.h>

#include <sys/time.h>
#include <time.h>

#if __STDC_VERSION__ < 201112L || __STDC_NO_ATOMICS == 1
#define HAS_ATOMICS 0
#else
#define HAS_ATOMICS 1
#endif

#if HAS_ATOMICS
#include <stdatomic.h>
#endif


size_t* linspace_uint(const size_t start, const size_t end, const size_t n)
{
    // assert(n >= 1);

    if (n < 1)
    {
        return NULL;
    }

    size_t* array = calloc(n + 1, sizeof(*array));

    const double delta = (double)(end - start) / n;

    for (size_t i = 0; i < n; ++i)
    {
        array[i] = lround(start + delta * i);
    }

    array[n] = end;

    return array;
}

int show_factor(const int k, const long int i)
{
    if (k > 0)
    {
        if (k > 1)
        {
            printf(" * %ld^%d", i, k);
        }
        else
        {
            printf(" * %ld", i);
        }
    }

    return 0;
}

/**
 * @brief Factorization of a number
 * @param n number to factorize
 */
void prime_factorization(long int n)
{

    printf("%ld = 1", n);

    int k = 0;
    while (n % 2 == 0)
    {
        n /= 2;
        ++k;
    }

    k = show_factor(k, 2);

    while (n % 3 == 0)
    {
        n /= 3;
        ++k;
    }

    k = show_factor(k, 3);

    for (long int i = 5; i <= n; i += 6)
    {
        while (n % i == 0)
        {
            n /= i;
            ++k;
        }

        k = show_factor(k, i);

        while (n % (i + 2) == 0)
        {
            n /= (i + 2);
            ++k;
        }

        k = show_factor(k, i + 2);
    }

    printf("\n");
}

long int largest_prime(long int n)
{
    if (n < 2)
    {
        return -1;
    }

    long int r = n;
    for (long int i = 2; i <= n; i++)
    {
        while (n % i == 0)
        {
            n /= i;
            r = i;
        }
    }

    return r;
}

bool isPrime(const long int n)
{
    if (n < 0)
    {
        return isPrime(-n);
    }

    if ((n == 2) || (n == 3))
    {
        return true;
    }

    if ((n < 2) || (n % 2 == 0) || (n % 3 == 0))
    {
        return false;
    }

    for (long int i = 5; i * i <= n; i += 6)
    {
        if ((n % i == 0) || (n % (i + 2) == 0))
        {
            return false;
        }
    }

    return true;
}

double elapsed_seconds(const struct timeval* begin)
{
    struct timeval end;
    gettimeofday(&end, NULL);

    return end.tv_sec - begin->tv_sec + (end.tv_usec - begin->tv_usec) * 1e-6;
}

void prime_numbers_below(const size_t n)
{
    struct timeval begin;
    gettimeofday(&begin, NULL);

    size_t count = 0;
    for (size_t i = 2; i < n; ++i)
    {
        if (isPrime(i))
        {
            ++count;
            // printf("Number %zu is prime.\n", i);
        }
    }

    const double elapsed = elapsed_seconds(&begin);

    printf("Found %zu prime numbers smaller than %zu; time: %f.\n", count, n,
        elapsed);
}

static mtx_t g_serial_mtx;

static atomic_size_t g_count_atomic = 0;
static size_t g_prime_count = 0;
static size_t g_thread_count = 0;
static size_t g_prime_threshold = 0;
static size_t* g_array = NULL;
static thrd_t* g_threads = NULL;

int run_thread(void* arg)
{
    const size_t n = *(size_t*)arg;

    size_t start = g_array[n];
    size_t end = g_array[n + 1];

    if (start < 2)
    {
        start = 2;
    }

    free(arg);

    printf("Thread %zu started; steps: %zu; %zu.\n", n, start, end);

    struct timeval begin;
    gettimeofday(&begin, NULL);

    for (size_t i = start; i < end; ++i)
    {
        if (isPrime(i))
        {
            ++g_count_atomic;
            //mtx_lock(&g_serial_mtx);
            ++g_prime_count;
            //mtx_unlock(&g_serial_mtx);
        }
    }

    const double elapsed = elapsed_seconds(&begin);

    printf("Thread %zu time: %f\n", n, elapsed);

    return n;
}

void prime_numbers_below_threaded(size_t thread_count, size_t n)
{
    g_thread_count = thread_count;
    g_prime_threshold = n;
    g_threads = calloc(g_thread_count, sizeof(*g_threads));

    g_array = linspace_uint(0, g_prime_threshold, g_thread_count);

    mtx_init(&g_serial_mtx, mtx_plain);

    for (size_t i = 0; i < g_thread_count; ++i)
    {
        size_t* arg = malloc(sizeof(*arg));
        *arg = i;

        thrd_create(&g_threads[i], run_thread, arg);
    }

    printf("Waiting for thread to complete...\n");

    for (size_t i = 0; i < g_thread_count; ++i)
    {
        int res;
        thrd_join(g_threads[i], &res);

        printf("Thread %zu completed with result %d.\n", i, res);
    }

    mtx_destroy(&g_serial_mtx);

    free(g_array);
    free(g_threads);

    printf("All threads done.\n");
    printf("Found %zu (%zu) prime numbers smaller than %zu.\n", g_prime_count,
        g_count_atomic, g_prime_threshold);
}

void test_linspace(void)
{
    printf("\n");
    size_t* array = linspace_uint(0, 44, 1);
    for (size_t i = 0; i < 1; ++i)
    {
        printf("Array: %zu, ", array[i]);
    }
    printf("\n");
    free(array);

    printf("\n");
    array = linspace_uint(0, 44, 2);
    for (size_t i = 0; i < 2; ++i)
    {
        printf("Array: %zu, ", array[i]);
    }
    printf("\n");
    free(array);

    printf("\n");
    array = linspace_uint(0, 44, 7);
    for (size_t i = 0; i < 7; ++i)
    {
        printf("Array: %zu, ", array[i]);
    }
    printf("\n");
    free(array);

    printf("\n");
    array = linspace_uint(0, 16, 8);
    for (size_t i = 0; i < 8; ++i)
    {
        printf("Array: %zu, ", array[i]);
    }
    printf("\n");
    free(array);

    printf("\n");
    array = linspace_uint(0, 20, 6);
    for (size_t i = 0; i < 6; ++i)
    {
        printf("Array: %zu, ", array[i]);
    }
    printf("\n");
    free(array);

    printf("\n");
    array = linspace_uint(5, 176, 11);
    for (size_t i = 0; i < 11; ++i)
    {
        printf("Array: %zu, ", array[i]);
    }
    printf("\n");
    free(array);
}

int main(const int argc, const char* const argv[])
{
    const long int n = 600851475143;
    printf("Is number %ld prime? = %s\n", n, isPrime(n) ? "true" : "false");
    printf("Largest prime for %ld = %ld\n", n, largest_prime(n));

    prime_factorization(n);

    size_t threshold = 400000;
    if (argc > 1)
    {
        threshold = atol(argv[1]);
    }

    prime_numbers_below(threshold);

    const size_t thread_count = 8;
    prime_numbers_below_threaded(thread_count, threshold);

    return 0;
}
