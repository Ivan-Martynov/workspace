#include <stdio.h>

#define add(a,b) ((a)+(b))

int main(int argc, char* argv[static argc]) {
    int x = 3;
    int y = 5;
    printf("Sum of %d and %d is %d\n", x, y, add(x, y));

    return 0;
}
