#include <stdarg.h>
#include <stdio.h>
#include <time.h>

int timestamp_printf(char *format, ...)
{
    va_list args;
    time_t system_time = time(NULL); // system time in seconds since epoch
    int return_value;

#if 0
    system_time = time(NULL); // system time in seconds since epoch
    char *timestr = ctime(&system_time); // timestamp
    timestr = ctime(&system_time); // timestamp
#endif

    printf("%s", ctime(&system_time)); // timestr has a newline

    va_start(args, format);

    return_value = vprintf(format, args);

    va_end(args);

    return return_value;
}

float average(int count, ...)
{
    float res = 0;
    int i, val;
    va_list args;

    va_start(args, count);

    for (i = 0; i < count; i++) {
        val = va_arg(args, int);
        res += (float)val;
    }

    va_end(args);

#if 0
    /* whatever happens here is ignored as a comment */
    int b = 23;
    float d = 0.2;
#endif
    return res / count;
}

int main(int argc, char **argv)
{
    float k = average(5, 1, 23, 42, 89, 72);
    timestamp_printf("%.2f\n", k);
}
