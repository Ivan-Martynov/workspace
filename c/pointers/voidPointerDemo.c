#include <stdio.h>

/* static - no one can see these except this file */
static void *pointer_array[10];
static int ind = 0;

void append_pointer(void *p)
{
    pointer_array[ind++] = p;
}

void *get_pointer(int i)
{
    return pointer_array[i];
}

int main(int argc, char **argv)
{
    char *s = "Sample text";
    int a = 10;
    int *b;

    char *s2;
    int *b2;

    b = &a;

    append_pointer(s);
    append_pointer(b);

    s2 = get_pointer(0);
    b2 = get_pointer(1);

    printf("%s %d\n", s2, *b2);

    return 0;
}
