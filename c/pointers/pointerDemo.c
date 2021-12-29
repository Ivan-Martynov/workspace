#include <stdio.h>

void get_string(int a, char **s)
{
    switch (a) {
        case 0:
            *s = "everybody";
            break;
        case 1:
            *s = "was";
            break;
        case 2:
            *s = "kung-foo fighting";
            break;
        default:
            *s = "errnnt";
    }
}

int main(int argc, char *argv[])
{
    int a[4] = {50, 99, 3490, 0};
    int *p = a;

    while (*p > 0) {
        printf("%i\n", *p);
        p++; // go to next int in memory
    }

    /* copy strings */
    char *s = "String to be copied";
    char dest[20];

    /* string pointers */
    char *sp, *dp;

    for (sp = s, dp = dest; *sp != '\0'; sp++, dp++)
        *dp = *sp;

    printf("%s\n", dest);

    get_string(2, &s);

    printf("%s\n", s);

    return 0;
}
