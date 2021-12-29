/* Demo for pointer to function */
#include <stdio.h>

int deliver_fruit(char *address, float speed)
{
    printf("Delivering fruit to %s at speed %.2f\n", address, speed);
}

int main()
{
    int (*p)(char*, float); // function pointer variable

    p = deliver_fruit; // points to the deliver_fruit()

    p("My house", 52.8387);

    return 0;
}
