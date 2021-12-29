#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char **p;
    int i;

    p = malloc(sizeof(char *) * 10);

    for (i = 0; i < 10; i++) {
        *(p + i) = malloc(30);
        sprintf(*(p + i), "This is string #%d", i);
    }

    for (i = 0; i < 10; i++)
        printf("%d: %s\n", i, p[i]);

    return 0;
}
