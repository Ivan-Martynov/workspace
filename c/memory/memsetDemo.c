/** memset example */

#include <stdio.h>
#include <string.h>

int main()
{
	char str[] = "Time to learn about memset() function";
	//printf("%lu\n", sizeof str);
	//memset(str, '-', strlen(str));
	memset(str, '-', sizeof str);
	puts(str);
	return 0;
}
