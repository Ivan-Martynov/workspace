#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char* str = malloc(100);
	printf("Allocated.\n");
	strcpy(str, "Hello!");
	printf("Copied.\n");
	str  = "World.";
	printf("Reassigned.\n");
	
    return 0;
}