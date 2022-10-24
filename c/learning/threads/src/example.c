#ifdef __STDC_NO_THREADS__
#error Threads are not supported.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

int first_run(void* arg)
{
    int* a = arg;

    printf("Running thread with argument %d.\n", *a);

    return 12;
}

int second_run(void* arg)
{
    int a = *(int*)arg;

    free(arg);

    printf("Running thread with argument %d.\n", a);

    return a;
}

int third_run(void* arg)
{
    (void)arg;

    thread_local static int foo = 10;
    //static int foo = 10;

    int x = foo;

    //printf("Thread %d running before being detached.\n", thrd_current());
    printf("Thread: x = %d, foo = %d.\n", x, foo);

    if (x != foo)
    {
        printf("Thread: smelly... x = %d, foo = %d.\n", x, foo);
    }

    ++foo;

    return 0;
}

void first_example(void)
{
    printf("First example.\n\n");
    printf("Launching a thread.\n");

    thrd_t t;
    int arg = 3490;
    thrd_create(&t, first_run, &arg);

    printf("Doing other stuff...\n");

    printf("Waiting for the thread to join...\n");

    int result;
    thrd_join(t, &result);

    printf("Thread returned with result %d.\n", result);
    printf("----------------------------\n\n");
}

void second_example(void)
{
    printf("Second example.\n\n");

    const size_t thread_count = 5;
    thrd_t threads[thread_count];

    printf("Launching threads.\n");

    for (size_t i = 0; i < thread_count; ++i)
    {
        int* arg = malloc(sizeof(*arg));
        *arg = i;

        thrd_create(threads + i, second_run, arg);
    }

    printf("Doing other stuff...\n");

    printf("Waiting for the threads to complete...\n");

    for (size_t i = 0; i < thread_count; ++i)
    {
        int result;
        thrd_join(threads[i], &result);
        printf("Thread %d done.\n", result);
    }

    printf("----------------------------\n\n");
}

void third_example(void)
{
    printf("Third example.\n\n");

    const size_t thread_count = 5;

    printf("Launching a thread.\n");

    thrd_t t;
    for (size_t i = 0; i < thread_count; ++i)
    {
        thrd_create(&t, third_run, NULL);
        thrd_detach(t);
    }

    thrd_sleep(&(struct timespec) {.tv_sec = 1}, NULL);
}

int main(void)
{
    first_example();

    second_example();

    third_example();

    return 0;
}